// #include "../../utils/wmi_wrapper.h"

#pragma once

// #ifdef HWINFO_WINDOWS

#include <algorithm>
#include <memory>
#include <vector>

#include "../../utils/stringutils.hpp"
// #    include "WMIwrapper.hpp"

#include <WbemIdl.h>
#include <comdef.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#pragma comment(lib, "wbemuuid.lib")

namespace hwinfo
{
    namespace utils
    {
        namespace WMI
        {

            class WMIWrapper
            {
            public:
                WMIWrapper()
                {

                    auto res = CoInitializeSecurity(nullptr,    // Security descriptor for the app (default if nullptr)
                                                    -1,         // Count of entries in asAuthSvc
                                                    nullptr,    // Array of auth services a server can use to call a client
                                                    nullptr,    // Reserved for future use
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
                                                    &m_service    // A pointer to a pointer to the destination IWbemServices interface.
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

                ~WMIWrapper()
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

                template< typename T >
                std::vector< T > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter = L"")
                {}

                template<>
                std::vector< long > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< long > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.intVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< int > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< int > result;
                    for (const auto& v : query< long >(wmi_class, field, filter)) {
                        result.push_back(static_cast< int >(v));
                    }
                    return result;
                }

                template<>
                std::vector< bool > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< bool > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.boolVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< unsigned > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< unsigned > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.uintVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< unsigned short > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< unsigned short > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.uiVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< long long > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< long long > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.llVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< unsigned long long >
                    query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::vector< unsigned long long > result;

                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
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
                        result.push_back(vt_prop.ullVal);
                        VariantClear(&vt_prop);
                        obj->Release();
                    }
                    return result;
                }

                template<>
                std::vector< std::string > query(const std::wstring& wmi_class, const std::wstring& field, const std::wstring& filter)
                {
                    std::wstring filter_string;
                    if (!filter.empty()) {
                        filter_string.append(L" WHERE " + filter);
                    }
                    std::wstring query_string(L"SELECT " + field + L" FROM " + wmi_class + filter_string);
                    bool         success = execute_query(query_string);
                    if (!success) {
                        return {};
                    }
                    std::vector< std::string > result;

                    ULONG             u_return = 0;
                    IWbemClassObject* obj      = nullptr;
                    while (m_enumerator) {
                        m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                        if (!u_return) {
                            break;
                        }
                        VARIANT vt_prop;
                        obj->Get(field.c_str(), 0, &vt_prop, nullptr, nullptr);
                        result.push_back(wstring_to_std_string(vt_prop.bstrVal));
                        VariantClear(&vt_prop);
                        obj->Release();
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

        }    // namespace WMI
    }        // namespace utils
}    // namespace hwinfo

// #endif  // WINDOWS