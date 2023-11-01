//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct OSInfo
     * @brief Represents operating system information obtained from WMI.
     */
    struct OSInfo
    {
        struct OSInfoType
        {
        };

        /**
         * @struct FREEMEMORY
         * @brief Encapsulates the FreePhysicalMemory property of CIM_OperatingSystem.
         */
        struct FREEMEMORY
        {
            using info_type   = OSInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"CIM_OperatingSystem";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"FreePhysicalMemory";     ///< WMI field for the free physical memory.
        };
    };
}    // namespace hwinfo::WMI
