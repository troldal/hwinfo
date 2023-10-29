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
    struct PerfInfo
    {
        /**
         * @struct PROCESSORPERFORMANCE
         * @brief
         */
        struct PROCESSORPERFORMANCE
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PerfFormattedData_Counters_ProcessorInformation";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"PercentProcessorPerformance";    ///< WMI field for the battery name.
        };

        /**
         * @struct PROCESSORUTILITY
         * @brief
         */
        struct PROCESSORUTILITY
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_PerfFormattedData_Counters_ProcessorInformation";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"PercentProcessorUtility";    ///< WMI field for the battery name.
        };
    };
}    // namespace hwinfo::WMI
