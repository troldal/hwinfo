

#pragma once

#include "../../base/batteryBase.hpp"
#include "WMIBatteryInfo.hpp"
#include "WMIBoardInfo.hpp"
#include "WMICpuInfo.hpp"
#include "WMIDiskInfo.hpp"
#include "WMIGpuInfo.hpp"
#include "WMIOSInfo.hpp"
#include "WMIPerfInfo.hpp"
#include "WMIRamInfo.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <variant>
#include <vector>

#include "../../utils/stringutils.hpp"

#include <WbemIdl.h>
#include <comdef.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#pragma comment(lib, "wbemuuid.lib")

namespace hwinfo::WMI
{
    class WMIInterface
    {
    public:
        WMIInterface()
        {
            auto res = CoInitializeSecurity(nullptr,                        // Security descriptor for the app (default if nullptr)
                                            -1,                             // Count of entries in asAuthSvc
                                            nullptr,                        // Array of auth services a server can use to call a client
                                            nullptr,                        // Reserved for future use
                                            RPC_C_AUTHN_LEVEL_DEFAULT,      // Default authentication level for the process
                                            RPC_C_IMP_LEVEL_IMPERSONATE,    // Default impersonation level for proxies
                                            nullptr,                        // Pointer to authentication array
                                            EOAC_NONE,                      // Additional capabilities of this application
                                            nullptr                         // Reserved for future use
            );

            res &= CoInitializeEx(nullptr,                // Reserved, must be NULL
                                  COINIT_MULTITHREADED    // Apartment threaded model
            );

            res &= CoCreateInstance(
                CLSID_WbemLocator,       // The CLSID associated with the data and code that will be used to create the object.
                nullptr,                 // Pointer to the aggregate (usually NULL).
                CLSCTX_INPROC_SERVER,    // The context (in this case, DLL runs in the same process).
                IID_IWbemLocator,        // Reference to the identifier of the interface.
                (LPVOID*)&m_locator      // Address of pointer variable that receives the interface pointer requested.
            );

            BSTR path = SysAllocString(L"ROOT\\CIMV2");
            res &= m_locator->ConnectServer(path,         // The path of the namespace for the connection.
                                            nullptr,                   // The name of the user.
                                            nullptr,                   // The password of the user.
                                            nullptr,                   // The locale identifier.
                                            0,                         // Security flags.
                                            nullptr,                   // The authority (for example, Kerberos).
                                            nullptr,                   // The context object.
                                            &m_service                 // A pointer to a pointer to the destination IWbemServices interface.
            );
            SysFreeString(path);

            res &= CoSetProxyBlanket(m_service,                      // Pointer to the object to be set
                                     RPC_C_AUTHN_WINNT,              // The authentication service to be used
                                     RPC_C_AUTHZ_NONE,               // The authorization service to be used
                                     nullptr,                        // The server principal name
                                     RPC_C_AUTHN_LEVEL_CALL,         // The authentication level to be used
                                     RPC_C_IMP_LEVEL_IMPERSONATE,    // The impersonation level to be used
                                     nullptr,                        // The authentication identity to be used
                                     EOAC_NONE                       // Capability flags
            );

            if (!SUCCEEDED(res)) throw std::runtime_error("error initializing WMI");
        }

        ~WMIInterface()
        {
            if (m_locator) m_locator->Release();
            if (m_service) m_service->Release();
            CoUninitialize();
        }

        bool execute_query(const std::wstring& query)
        {
            BSTR bstr_wql = SysAllocString(L"WQL");
            BSTR bstr_sql = SysAllocString(query.c_str());

            auto result = SUCCEEDED(
                m_service->ExecQuery(bstr_wql, bstr_sql, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &m_enumerator));

            SysFreeString(bstr_wql);
            SysFreeString(bstr_sql);
            return result;
        }

        /**
         * @brief Executes a WMI query and retrieves results as a vector of tuples.
         *
         * This function builds a WMI query string based on the provided template parameters,
         * executes the query, and converts the results into a vector of tuples. Each template
         * parameter should be a struct representing a WMI class field with specific attributes.
         *
         * @tparam Types Variadic template parameters, where each type represents a WMI class field.
         * @return QueryResult A vector of tuples containing the results of the WMI query.
         */
        template< typename... Types >
        auto query()
        {
            // Define the type of the resulting vector of tuples.
            using QueryResult = std::vector< std::tuple< typename Types::result_type... > >;
            QueryResult queryResults;

            // Lambda to build the WMI query string.
            auto buildQueryString = [&]() -> std::wstring {
                std::wstring queryString = L"SELECT ";
                (..., (queryString += Types::wmi_field + L", "));
                queryString.pop_back();    // Remove trailing comma and space
                queryString.pop_back();
                queryString += L" FROM " + std::tuple_element_t< 0, std::tuple< Types... > >::wmi_class;
                return queryString;
            };

            // Lambda to convert a VARIANT to the specific result type.
            auto convertVariant = []< typename T >(const VARIANT& val, T type) {
                if constexpr (std::is_same_v< typename T::value_type, int32_t >) return static_cast< typename T::result_type >(val.intVal);
                else if constexpr (std::is_same_v< typename T::value_type, bool >)
                    return val.boolVal;
                else if constexpr (std::is_same_v< typename T::value_type, uint32_t >)
                    return static_cast< typename T::result_type >(val.uintVal);
                else if constexpr (std::is_same_v< typename T::value_type, uint16_t >)
                    return static_cast< typename T::result_type >(val.uiVal);
                else if constexpr (std::is_same_v< typename T::value_type, int64_t >)
                    return static_cast< typename T::result_type >(val.llVal);
                else if constexpr (std::is_same_v< typename T::value_type, uint64_t >)
                    return static_cast< typename T::result_type >(val.ullVal);
                else if constexpr (std::is_same_v< typename T::value_type, std::string >)
                    return utils::wstring_to_std_string(val.bstrVal);
                else
                    std::invoke([]< bool flag = false >() { static_assert(flag, "unsupported type"); });
            };

            // Lambda to retrieve a property from a WMI class object.
            auto retrieveProperty = [&](IWbemClassObject* pObj, const std::wstring& propertyName) -> VARIANT {
                VARIANT vtProperty;
                pObj->Get(propertyName.c_str(), 0, &vtProperty, nullptr, nullptr);
                return vtProperty;
            };

            // Lambda to populate the results vector.
            auto populateQueryResults = [&]() {
                ULONG             uReturned = 0;
                IWbemClassObject* pObj      = nullptr;

                // Iterate through the results of the WMI query.
                while (m_enumerator) {
                    m_enumerator->Next(WBEM_INFINITE, 1, &pObj, &uReturned);
                    if (!uReturned) break;

                    // Convert the properties of the object to the result types and add them to the results vector.
                    // For each type in Types, retrieve the property, convert it, and add it to the tuple.
                    queryResults.emplace_back(std::make_tuple(convertVariant(retrieveProperty(pObj, Types::wmi_field), Types {})...));
                    pObj->Release();    // Release the object
                }
            };

            // Build the query string and execute the query.
            const std::wstring queryString = buildQueryString();
            if (!execute_query(queryString)) {
                return queryResults;    // Return an empty result if the query execution fails
            }

            // Populate the results vector.
            populateQueryResults();
            return queryResults;    // Return the results
        }

        //            private:
        //                std::unique_ptr< IWbemLocator, decltype([](IWbemLocator* p) { p->Release(); }) >                 m_locator
        //                {}; std::unique_ptr< IWbemServices, decltype([](IWbemServices* p) { p->Release(); }) > m_service {};
        //                std::unique_ptr< IEnumWbemClassObject, decltype([](IEnumWbemClassObject* p) { p->Release(); }) >
        //                m_enumerator {};
        IWbemLocator*         m_locator {};
        IWbemServices*        m_service {};
        IEnumWbemClassObject* m_enumerator {};
    };


}    // namespace hwinfo::WMI
