//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct CpuInfo
     * @brief Represents battery information obtained from WMI.
     */
    struct RamInfo
    {
        /**
         * @struct NAME
         * @brief
         */
        struct NAME
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";                    ///< WMI field for the battery name.
        };

        /**
         * @struct MANUFACTURER
         * @brief
         */
        struct MANUFACTURER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";            ///< WMI field for the battery name.
        };

        /**
         * @struct PARTNUMBER
         * @brief
         */
        struct PARTNUMBER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"PartNumber";              ///< WMI field for the battery name.
        };

        /**
         * @struct SERIALNUMBER
         * @brief
         */
        struct SERIALNUMBER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"SerialNumber";            ///< WMI field for the battery name.
        };

        /**
         * @struct CAPACITY
         * @brief
         */
        struct CAPACITY
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Capacity";                ///< WMI field for the battery name.
        };
    };
}    // namespace hwinfo::WMI
