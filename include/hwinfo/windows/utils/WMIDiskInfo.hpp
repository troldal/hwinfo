//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct DiskInfo
     * @brief Represents disk drive information obtained from WMI.
     */
    struct DiskInfo
    {
        /**
         * @struct MODEL
         * @brief Encapsulates the Model property of Win32_DiskDrive.
         */
        struct MODEL
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_DiskDrive";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Model";              ///< WMI field for the disk drive model.
        };

        /**
         * @struct MANUFACTURER
         * @brief Encapsulates the Manufacturer property of Win32_DiskDrive.
         */
        struct MANUFACTURER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_DiskDrive";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";       ///< WMI field for the disk drive manufacturer.
        };

        /**
         * @struct SERIALNUMBER
         * @brief Encapsulates the SerialNumber property of Win32_DiskDrive.
         */
        struct SERIALNUMBER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_DiskDrive";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"SerialNumber";       ///< WMI field for the disk drive serial number.
        };

        /**
         * @struct SIZE
         * @brief Encapsulates the Size property of Win32_DiskDrive.
         */
        struct SIZE
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_DiskDrive";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Size";               ///< WMI field for the size of the disk drive.
        };
    };
}    // namespace hwinfo::WMI