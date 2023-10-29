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
    struct OSInfo
    {
        /**
         * @struct FREEMEMORY
         * @brief
         */
        struct FREEMEMORY
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"CIM_OperatingSystem";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"FreePhysicalMemory";     ///< WMI field for the battery name.
        };
    };
}    // namespace hwinfo::WMI
