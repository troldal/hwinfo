/*

    88        88  I8,        8        ,8I  88                 ad88
    88        88  `8b       d8b       d8'  88                d8"
    88        88   "8,     ,8"8,     ,8"   88                88
    88aaaaaaaa88    Y8     8P Y8     8P    88  8b,dPPYba,  MM88MMM  ,adPPYba,
    88""""""""88    `8b   d8' `8b   d8'    88  88P'   `"8a   88    a8"     "8a
    88        88     `8a a8'   `8a a8'     88  88       88   88    8b       d8
    88        88      `8a8'     `8a8'      88  88       88   88    "8a,   ,a8"
    88        88       `8'       `8'       88  88       88   88     `"YbbdP"'

    Based on the work of:
    Leon Freist <freist@informatik.uni-freiburg.de>

    Copyright © 2022 Leon Freist
    Copyright © 2023 Kenneth Troldal Balslev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the “Software”), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished
    to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

// ===== Internal Includes
#include "../../base/batteryBase.hpp"
#include "../../utils/stringutils.hpp"
#include "WMIBatteryInfo.hpp"
#include "WMIBoardInfo.hpp"
#include "WMICpuInfo.hpp"
#include "WMIDiskInfo.hpp"
#include "WMIGpuInfo.hpp"
#include "WMIOSInfo.hpp"
#include "WMIPerfInfo.hpp"
#include "WMIRamInfo.hpp"

// ===== External Includes
#include <WbemIdl.h>
#include <comdef.h>

// ===== Standard Library Includes
#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// Issue/Improvement List for WMIInterface.hpp
//
// 2. COM Initialization and Cleanup:
//    - Consider separating the COM initialization and cleanup into their own methods for clarity and reusability.
//    - Check the return values of CoInitializeSecurity and CoInitializeEx explicitly, instead of using bitwise AND operator.
//
// 3. Error Handling and Resource Management:
//    - Improve error handling by providing more descriptive error messages.
//    - Use _com_error to translate HRESULT values to human-readable strings.
//    - Ensure all resources (e.g., BSTR) are properly released or managed using RAII principles (e.g., std::unique_ptr).
//
// 4. Query Execution and Result Handling:
//    - Consider handling special cases where the WMI query might return NULL or unexpected types.
//
// 7. Query String Construction:
//    - The buildQueryString method constructs the query string using tuple_element_t, which assumes that all Types have the same wmi_class.
//      This may not be the case for all queries, and could lead to incorrect query strings.

/**
 * @file WMIInterface.hpp
 * @namespace hwinfo::WMI
 * @brief Contains the WMIInterface class for interacting with Windows Management Instrumentation.
 */
namespace hwinfo::WMI
{

    /**
     * @class WMIInterface
     * @brief The WMIInterface class provides an interface to Windows Management Instrumentation (WMI).
     *
     * This class allows for querying the WMI to retrieve various pieces of hardware and system information.
     * It encapsulates the necessary setup and querying processes, and provides a templated query function
     * to retrieve results in a type-safe manner.
     */
    class WMIInterface
    {
        /**
         * @brief Function object to release a BSTR.
         */
        struct BSTRDeleter
        {
            void operator()(BSTR ptr) const { SysFreeString(ptr); }
        };

        /**
         * @brief Type alias for a unique pointer to a BSTR, using the custom deleter.
         */
        using BSTRPtr = std::unique_ptr< std::remove_pointer< BSTR >::type, BSTRDeleter >;

    public:
        /**
         * @brief Default constructor for WMIInterface.
         * Initializes the COM library and sets up the WMI connection.
         * @throw std::runtime_error If WMI initialization fails.
         */
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

            // Convert the path to BSTR for WMI compatibility.
            BSTRPtr path(SysAllocString(L"ROOT\\CIMV2"));
            res &= m_locator->ConnectServer(path.get(),    // The path of the namespace for the connection.
                                            nullptr,       // The name of the user.
                                            nullptr,       // The password of the user.
                                            nullptr,       // The locale identifier.
                                            0,             // Security flags.
                                            nullptr,       // The authority (for example, Kerberos).
                                            nullptr,       // The context object.
                                            &m_service     // A pointer to a pointer to the destination IWbemServices interface.
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

        /**
         * @brief Destructor for WMIInterface.
         * Releases the WMI resources and uninitializes the COM library.
         */
        ~WMIInterface()
        {
            if (m_locator) m_locator->Release();
            if (m_service) m_service->Release();
            if (m_enumerator) m_enumerator->Release();
            CoUninitialize();
        }

        /**
         * @brief Executes a WMI query and returns the results.
         * @tparam Types Variadic template types representing the data types to be queried.
         * @return QueryResult A vector of tuples containing the query results.
         */
        template< typename... Types >
        auto query()
        {
            // Lock the mutex to ensure thread safety.
            std::lock_guard< std::mutex > lock(m_wmiMutex);

            // Define a type alias for the query result, which is a vector of tuples.
            using QueryResult = std::vector< std::tuple< typename Types::result_type... > >;
            QueryResult queryResults;

            // Build the WMI query string based on the specified types.
            const std::wstring queryString = buildQueryString< Types... >();

            // Execute the WMI query. If it fails, return an empty result set.
            if (!execQuery(queryString)) {
                return queryResults;
            }

            // Populate the query results based on the executed query.
            populateQueryResults< Types... >(queryResults);

            // Return the populated query results.
            return queryResults;
        }

    private:
        /**
         * @brief Executes a WMI query based on the given query string.
         * @param queryString The WMI query string.
         * @return true If the query was successful, false otherwise.
         */
        bool execQuery(const std::wstring& queryString)
        {
            // Convert the query language and string to BSTR for WMI compatibility.
            BSTRPtr bstr_wql(SysAllocString(L"WQL"));
            BSTRPtr bstr_sql(SysAllocString(queryString.c_str()));

            // Return false if the conversion failed.
            if (!bstr_wql || !bstr_sql) {
                return false;
            }

            // Execute the query using the WMI service and store the results in the enumerator.
            auto result = SUCCEEDED(m_service->ExecQuery(bstr_wql.get(),
                                                         bstr_sql.get(),
                                                         WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                                         nullptr,
                                                         &m_enumerator));

            // Return true if the query was successful, false otherwise.
            return result;
        }

        /**
         * @brief Populates the query results based on the executed query.
         * @tparam Types Variadic template types representing the data types to be queried.
         * @tparam T The type of the container to store the query results.
         * @param queryResults Container to store the query results.
         */
        template< typename... Types, typename T >
        void populateQueryResults(T& queryResults)
        {
            ULONG             uReturned = 0;          // Number of objects returned in each call
            IWbemClassObject* pObj      = nullptr;    // Pointer to hold each individual object

            // Return if the enumerator is null.
            if (!m_enumerator) return;

            // Iterate through the enumerator to retrieve all query results.
            while (m_enumerator) {
                // Retrieve the next object in the enumerator.
                m_enumerator->Next(WBEM_INFINITE, 1, &pObj, &uReturned);

                // Break the loop if no more objects are returned.
                if (!uReturned) break;

                // Convert the properties of the current object and add them to the result container.
                queryResults.emplace_back(std::make_tuple(convertVariant< Types >(retrieveProperty(pObj, Types::wmi_field))...));

                // Release the current object to prevent memory leaks.
                pObj->Release();
            }
        };

        /**
         * @brief Builds the WMI query string based on the template types provided.
         * @tparam Types Variadic template types representing the data types to be queried.
         * @return std::wstring The constructed WMI query string.
         */
        template< typename... Types >
        static std::wstring buildQueryString()
        {
            // Append all field names to the query string, separated by commas.
            std::wstring queryString = L"SELECT ";
            (..., (queryString += Types::wmi_field + L","));
            queryString.pop_back();    // Remove trailing comma

            // Specify the class from which to select the fields.
            queryString += L" FROM " + std::tuple_element_t< 0, std::tuple< Types... > >::wmi_class;

            // Return the constructed query string.
            return queryString;
        };

        /**
         * @brief Retrieves a property value from a WMI class object.
         * @param pObj Pointer to the WMI class object.
         * @param propertyName The name of the property to retrieve.
         * @return VARIANT The retrieved property value.
         */
        static VARIANT retrieveProperty(IWbemClassObject* pObj, const std::wstring& propertyName)
        {
            VARIANT vtProperty;

            // Retrieve the specified property from the WMI class object.
            pObj->Get(propertyName.c_str(), 0, &vtProperty, nullptr, nullptr);

            // Return the retrieved property value.
            return vtProperty;
        };

        /**
         * @brief Static assertion function to generate a compile-time error for unsupported types.
         * @tparam flag Compile-time boolean flag. Should always be false.
         */
        template< bool flag = false >
        static void typeError()
        {
            // This function should never be instantiated; it's purpose is to generate a compile-time error.
            static_assert(flag, "unsupported type");
        };

        /**
         * @brief Converts a VARIANT type to the corresponding C++ type.
         * @tparam T The C++ type to convert to.
         * @param val The VARIANT value to convert.
         * @return auto The converted C++ value.
         */
        template< typename T >
        static auto convertVariant(const VARIANT& val)
        {
            // Convert the VARIANT type to the corresponding C++ type.
            // The actual conversion depends on the type of T.

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
                // Generate a compile-time error for unsupported types.
                typeError();
        };

        IWbemLocator*         m_locator {};       ///< Pointer to the WBEM locator.
        IWbemServices*        m_service {};       ///< Pointer to the WBEM services.
        IEnumWbemClassObject* m_enumerator {};    ///< Pointer to the WBEM class object enumerator.

        std::mutex m_wmiMutex;    ///< Mutex to ensure thread safety.
    };

}    // namespace hwinfo::WMI
