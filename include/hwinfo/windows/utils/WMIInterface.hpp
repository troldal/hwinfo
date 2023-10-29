

#pragma once

#include "../../base/batteryBase.hpp"
#include "WMIBatteryInfo.hpp"
#include "WMICpuInfo.hpp"
#include "WMIPerfInfo.hpp"

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

        /**
         * @brief A metafunction to create a tuple type where each element is a vector of the associated type's `result_type`.
         *
         * This structure takes a variadic number of types as template parameters, and for each type, it retrieves the associated
         * `result_type` and creates a `std::vector` of that type. All these vectors are then used to form a `std::tuple`.
         *
         * @tparam Types Variadic template parameters representing the types for which the `result_type` is to be extracted.
         */
        template< typename... Types >
        struct MakeResultTuple
        {
            /**
             * A `std::tuple` type where each element is a `std::vector` of the associated type's `result_type`.
             */
            using type = std::tuple< std::vector< typename Types::result_type >... >;
        };

        /**
         * @brief A type alias for `MakeResultTuple<Types...>::type`.
         *
         * This type alias simplifies the usage of the `MakeResultTuple` metafunction, allowing for cleaner and more concise code.
         *
         * @tparam Types Variadic template parameters passed on to `MakeResultTuple`.
         */
        template< typename... Types >
        using ResultTuple = typename MakeResultTuple< Types... >::type;

        /**
         * @brief Queries values for a variety of types and transforms the results into a vector of tuples.
         *
         * This function is designed to work with a variadic number of types. For each type in the template parameter pack,
         * it queries a vector of values. The query is performed by calling the `queryValue<T>` function, where `T` is the
         * type from the template parameter pack. The result of each query is expected to be a `std::vector<T::result_type>`.
         *
         * After querying the values, this function transforms the collected vectors into a single `std::vector` of `std::tuple`.
         * Each tuple in the result vector corresponds to a specific index in the input vectors. The ith tuple contains the ith
         * element from each of the input vectors.
         *
         * @tparam Types Variadic template parameter pack representing the types for which values are queried.
         * @return std::vector<std::tuple<typename Types::result_type...>> A vector of tuples holding the queried values.
         *
         * @note It is assumed that all vectors returned from `queryValue<T>` calls have the same size. If they have different sizes,
         *       the behavior is undefined.
         */
        template< typename... Types >
        auto queryRecord()
        {
            // ResultTuple is a tuple of vectors, where each vector's type is T::result_type for each type T in Types...
            ResultTuple< Types... > myTuple;

            // Lambda function to populate each vector in the tuple.
            // It takes an index_sequence as a template parameter,
            // which generates a compile-time sequence of indices.
            auto populateTuple = [&]< std::size_t... Is >(std::index_sequence< Is... >) {
                ((std::get< Is >(myTuple) = queryValue< Types >()), ...);
            };

            // Call the lambda function with an index sequence generated for Types...
            populateTuple(std::index_sequence_for< Types... > {});

            // Assume all vectors in the tuple have the same size.
            const size_t size = std::get< 0 >(myTuple).size();    // Assuming all vectors have the same size

            // Define the type of the resulting vector of tuples.
            using ResultVector = std::vector< std::tuple< typename Types::result_type... > >;
            ResultVector result;
            result.reserve(size);

            // Convert the tuple of vectors to a vector of tuples.
            for (size_t i = 0; i < size; ++i) {
                // Lambda function to emplace a tuple into the result vector.
                // The tuple is constructed using the ith element from each vector in the tuple.
                auto emplaceTupleElement = [&]< std::size_t... Is >(std::index_sequence< Is... >) {
                    // For each index in Is, get the ith element from the corresponding
                    // vector in the tuple and emplace_back a tuple into the result vector.
                    result.emplace_back(std::get< Is >(myTuple)[i]...);
                };

                // Call the lambda function with an index sequence generated for Types...
                emplaceTupleElement(std::index_sequence_for< Types... > {});
            }

            return result;
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
