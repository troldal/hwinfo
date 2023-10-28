

#pragma once

#include "../../base/batteryBase.hpp"
#include "WMIBatteryInfo.hpp"

#include <algorithm>
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

            res &= m_locator->ConnectServer(_bstr_t("ROOT\\CIMV2"),    // The path of the namespace for the connection.
                                            nullptr,                   // The name of the user.
                                            nullptr,                   // The password of the user.
                                            nullptr,                   // The locale identifier.
                                            0,                         // Security flags.
                                            nullptr,                   // The authority (for example, Kerberos).
                                            nullptr,                   // The context object.
                                            &m_service                 // A pointer to a pointer to the destination IWbemServices interface.
            );

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
            return SUCCEEDED(m_service->ExecQuery(bstr_t(L"WQL"),
                                                  bstr_t(std::wstring(query.begin(), query.end()).c_str()),
                                                  WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                                  nullptr,
                                                  &m_enumerator));
        }

        template< typename T, typename = std::enable_if_t< std::is_integral< T >::value || std::is_same< T, std::string >::value > >
        std::vector< T > query(const std::string& cls, const std::string& fields, const std::string& filter = "")
        {
            std::wstring wmi_class  = utils::NarrowStringToWideString(cls);
            std::wstring field      = utils::NarrowStringToWideString(fields);
            std::wstring wmi_filter = utils::NarrowStringToWideString(filter);

            std::vector< T > result;

            std::wstring filter_string;
            if (!wmi_filter.empty()) {
                filter_string.append(L" WHERE " + wmi_filter);
            }
            std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
            bool         success = execute_query(query_string);
            if (!success) {
                return {};
            }

            ULONG             u_return = 0;
            IWbemClassObject* obj      = nullptr;
            while (m_enumerator) {
                m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                if (!u_return) {
                    break;
                }
                VARIANT vt_prop;
                obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);

                if constexpr (std::is_same_v< T, long >) result.push_back(vt_prop.intVal);
                else if constexpr (std::is_same_v< T, int > || std::is_same_v< T, int32_t >)
                    result.push_back(static_cast< int >(vt_prop.intVal));
                else if constexpr (std::is_same_v< T, bool >)
                    result.push_back(vt_prop.boolVal);
                else if constexpr (std::is_same_v< T, unsigned > || std::is_same_v< T, uint32_t >)
                    result.push_back(vt_prop.uintVal);
                else if constexpr (std::is_same_v< T, unsigned short > || std::is_same_v< T, uint16_t >)
                    result.push_back(vt_prop.uiVal);
                else if constexpr (std::is_same_v< T, long long > || std::is_same_v< T, int64_t >)
                    result.push_back(vt_prop.llVal);
                else if constexpr (std::is_same_v< T, unsigned long long > || std::is_same_v< T, uint64_t >)
                    result.push_back(vt_prop.ullVal);
                else if constexpr (std::is_same_v< T, std::string >)
                    result.push_back(utils::wstring_to_std_string(vt_prop.bstrVal));
                else
                    std::invoke([]< bool flag = false >() { static_assert(flag, "unsupported type"); });

                VariantClear(&vt_prop);
                obj->Release();
            }
            return result;
        }

        template< typename T >
        std::vector< typename T::result_type > queryValue()
        {
            std::wstring wmi_class = T::wmi_class;
            std::wstring field     = T::wmi_field;

            std::vector< typename T::result_type > result;

            std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class);
            if (!execute_query(query_string)) return {};

            ULONG             u_return = 0;
            IWbemClassObject* obj      = nullptr;
            while (m_enumerator) {
                m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                if (!u_return) break;

                VARIANT vt_prop;
                obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);

                if constexpr (std::is_same_v< typename T::value_type, long >) result.push_back(vt_prop.intVal);
                else if constexpr (std::is_same_v< typename T::value_type, int > || std::is_same_v< typename T::value_type, int32_t >)
                    result.push_back(static_cast< typename T::result_type >(vt_prop.intVal));
                else if constexpr (std::is_same_v< typename T::value_type, bool >)
                    result.push_back(vt_prop.boolVal);
                else if constexpr (std::is_same_v< typename T::value_type, unsigned > || std::is_same_v< typename T::value_type, uint32_t >)
                    result.push_back(static_cast< typename T::result_type >(vt_prop.uintVal));
                else if constexpr (std::is_same_v< typename T::value_type, unsigned short > ||
                                   std::is_same_v< typename T::value_type, uint16_t >)
                    result.push_back(static_cast< typename T::result_type >(vt_prop.uiVal));
                else if constexpr (std::is_same_v< typename T::value_type, long long > || std::is_same_v< typename T::value_type, int64_t >)
                    result.push_back(static_cast< typename T::result_type >(vt_prop.llVal));
                else if constexpr (std::is_same_v< typename T::value_type, unsigned long long > ||
                                   std::is_same_v< typename T::value_type, uint64_t >)
                    result.push_back(static_cast< typename T::result_type >(vt_prop.ullVal));
                else if constexpr (std::is_same_v< typename T::value_type, std::string >)
                    result.push_back(utils::wstring_to_std_string(vt_prop.bstrVal));
                else
                    std::invoke([]< bool flag = false >() { static_assert(flag, "unsupported type"); });

                VariantClear(&vt_prop);
                obj->Release();
            }
            return result;
        }

        template< typename... Types >
        struct MakeResultTuple
        {
            using type = std::tuple< std::vector< typename Types::result_type >... >;
        };

        template< typename... Types >
        using ResultTuple = typename MakeResultTuple< Types... >::type;

        template< typename... Types >
        auto queryRecord()
        {
            using inpTuple = std::tuple< Types... >;
            ResultTuple< Types... > myTuple;

            [&]< std::size_t... Indices >(std::index_sequence< Indices... >) {
                ((std::get< Indices >(myTuple) = queryValue< std::tuple_element_t< Indices, inpTuple > >()), ...);
            }(std::index_sequence_for< Types... > {});

            return myTuple;
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

    //
    //    template< BatteryQuery Q >
    //    auto batteryQuery(IWbemClassObject* WMIObject)
    //    {
    //        VARIANT vt_prop;
    //
    //        if constexpr (Q == BatteryQuery::Name) {
    //            WMIObject->Get(L"Name", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return utils::wstring_to_std_string(vt_prop.bstrVal);
    //        }
    //
    //        if constexpr (Q == BatteryQuery::Description) {
    //            WMIObject->Get(L"Description", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return utils::wstring_to_std_string(vt_prop.bstrVal);
    //        }
    //        if constexpr (Q == BatteryQuery::Chemistry) {
    //            WMIObject->Get(L"Chemistry", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return vt_prop.iVal;
    //        }
    //        if constexpr (Q == BatteryQuery::BatteryStatus) {
    //            WMIObject->Get(L"BatteryStatus", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return vt_prop.iVal;
    //        }
    //        if constexpr (Q == BatteryQuery::Status) {
    //            WMIObject->Get(L"Status", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return utils::wstring_to_std_string(vt_prop.bstrVal);
    //        }
    //        if constexpr (Q == BatteryQuery::FullChargeCapacity) {
    //            WMIObject->Get(L"FullChargeCapacity", 0, &vt_prop, nullptr, nullptr);
    //            VariantClear(&vt_prop);
    //            return vt_prop.uintVal;
    //        }
    //    }
    //
    //    std::vector< detail::BatteryData > loadBatteryData()
    //    {
    //        utils::WMIWrapper wmi {};
    //
    //        const std::wstring query_string(
    //            L"SELECT Name, Description, Chemistry, BatteryStatus, Status, FullChargeCapacity FROM Win32_Battery");
    //        bool success = wmi.execute_query(query_string);
    //        if (!success) return {};
    //
    //        std::vector< detail::BatteryData > batteries;
    //
    //        ULONG             u_return  = 0;
    //        IWbemClassObject* WMIObject = nullptr;
    //
    //        while (wmi.m_enumerator) {
    //            wmi.m_enumerator->Next(WBEM_INFINITE, 1, &WMIObject, &u_return);
    //            if (!u_return) break;
    //
    //            detail::BatteryData battery;
    //
    //            battery.name        = batteryQuery< BatteryQuery::Name >(WMIObject);
    //            battery.description = batteryQuery< BatteryQuery::Description >(WMIObject);
    //            battery.technology  = static_cast< detail::BatteryTechnology >(batteryQuery< BatteryQuery::Chemistry >(WMIObject));
    //            battery.status      = static_cast< detail::BatteryStatus >(batteryQuery< BatteryQuery::BatteryStatus >(WMIObject));
    //            battery.health      = batteryQuery< BatteryQuery::Status >(WMIObject);
    //            battery.capacity    = batteryQuery< BatteryQuery::FullChargeCapacity >(WMIObject);
    //
    //            WMIObject->Release();
    //            batteries.push_back(std::move(battery));
    //        }
    //        return batteries;
    //    }

}    // namespace hwinfo::WMI
