//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct BoardInfo
     * @brief Represents baseboard (motherboard) information obtained from WMI.
     */
    struct BoardInfo
    {
        /**
         * @struct MANUFACTURER
         * @brief Encapsulates the Manufacturer property of Win32_BaseBoard.
         */
        struct MANUFACTURER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";       ///< WMI field for the manufacturer of the baseboard.
        };

        /**
         * @struct PRODUCT
         * @brief Encapsulates the Product property of Win32_BaseBoard.
         */
        struct PRODUCT
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Product";            ///< WMI field for the product name of the baseboard.
        };

        /**
         * @struct VERSION
         * @brief Encapsulates the Version property of Win32_BaseBoard.
         */
        struct VERSION
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Version";            ///< WMI field for the version of the baseboard.
        };

        /**
         * @struct SERIALNUMBER
         * @brief Encapsulates the SerialNumber property of Win32_BaseBoard.
         */
        struct SERIALNUMBER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_BaseBoard";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"SerialNumber";       ///< WMI field for the serial number of the baseboard.
        };
    };
}    // namespace hwinfo::WMI
