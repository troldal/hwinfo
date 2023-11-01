//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct PerfInfo
     * @brief Represents performance information obtained from WMI.
     */
    struct PerfInfo
    {
        struct PerfInfoType
        {
        };

        /**
         * @struct PROCESSORPERFORMANCE
         * @brief Encapsulates the PercentProcessorPerformance property of Win32_PerfFormattedData_Counters_ProcessorInformation.
         */
        struct PROCESSORPERFORMANCE
        {
            using info_type   = PerfInfoType;
            using value_type  = std::string;    ///< Type of the value being used. This might need to be changed to a numeric type.
            using result_type = std::string;    ///< Resultant type after conversion. This might need to be changed to a numeric type.

            static const inline std::wstring wmi_class = L"Win32_PerfFormattedData_Counters_ProcessorInformation";    ///< WMI class name.
            static const inline std::wstring wmi_field =
                L"PercentProcessorPerformance";    ///< WMI field for the processor performance percentage.
        };

        /**
         * @struct PROCESSORUTILITY
         * @brief Encapsulates the PercentProcessorUtility property of Win32_PerfFormattedData_Counters_ProcessorInformation.
         */
        struct PROCESSORUTILITY
        {
            using info_type   = PerfInfoType;
            using value_type  = std::string;    ///< Type of the value being used. This might need to be changed to a numeric type.
            using result_type = std::string;    ///< Resultant type after conversion. This might need to be changed to a numeric type.

            static const inline std::wstring wmi_class = L"Win32_PerfFormattedData_Counters_ProcessorInformation";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"PercentProcessorUtility";    ///< WMI field for the processor utility percentage.
        };
    };
}    // namespace hwinfo::WMI
