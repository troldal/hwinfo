//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct RamInfo
     * @brief Represents RAM (Random Access Memory) information obtained from WMI.
     */
    class RamInfo
    {
        struct RamInfoType
        {
        };

    public:
        /**
         * @struct NAME
         * @brief Encapsulates the Name property of Win32_PhysicalMemory.
         */
        struct NAME
        {
            using info_type   = RamInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";                    ///< WMI field for the name of the RAM module.
        };

        /**
         * @struct MANUFACTURER
         * @brief Encapsulates the Manufacturer property of Win32_PhysicalMemory.
         */
        struct MANUFACTURER
        {
            using info_type   = RamInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";            ///< WMI field for the manufacturer of the RAM module.
        };

        /**
         * @struct PARTNUMBER
         * @brief Encapsulates the PartNumber property of Win32_PhysicalMemory.
         */
        struct PARTNUMBER
        {
            using info_type   = RamInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"PartNumber";              ///< WMI field for the part number of the RAM module.
        };

        /**
         * @struct SERIALNUMBER
         * @brief Encapsulates the SerialNumber property of Win32_PhysicalMemory.
         */
        struct SERIALNUMBER
        {
            using info_type   = RamInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"SerialNumber";            ///< WMI field for the serial number of the RAM module.
        };

        /**
         * @struct CAPACITY
         * @brief Encapsulates the Capacity (in bytes) property of Win32_PhysicalMemory.
         */
        struct CAPACITY
        {
            using info_type   = RamInfoType;
            using value_type  = std::string;    ///< Type of the value being used. Consider using a numeric type for memory size.
            using result_type = std::string;    ///< Resultant type after conversion. Consider using a numeric type for memory size.

            static const inline std::wstring wmi_class = L"Win32_PhysicalMemory";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Capacity";                ///< WMI field for the capacity of the RAM module.
        };
    };
}    // namespace hwinfo::WMI
