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
    struct BoardInfo
    {
        /**
         * @struct MANUFACTURER
         * @brief
         */
        struct MANUFACTURER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";       ///< WMI field for the battery name.
        };

        /**
         * @struct PRODUCT
         * @brief
         */
        struct PRODUCT
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Product";            ///< WMI field for the battery name.
        };

        /**
         * @struct VERSION
         * @brief
         */
        struct VERSION
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Version";            ///< WMI field for the battery name.
        };

        /**
         * @struct SERIALNUMBER
         * @brief
         */
        struct SERIALNUMBER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"SerialNumber";       ///< WMI field for the battery name.
        };
    };
}    // namespace hwinfo::WMI
