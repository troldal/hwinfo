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
     * @brief Represents CPU (Central Processing Unit) information obtained from WMI.
     */
    struct CpuInfo
    {
        struct CpuInfoType
        {
        };

        /**
         * @struct NAME
         * @brief Encapsulates the Name property of Win32_Processor.
         */
        struct NAME
        {
            using info_type   = CpuInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";               ///< WMI field for the CPU name.
        };

        /**
         * @struct MANUFACTURER
         * @brief Encapsulates the Manufacturer property of Win32_Processor.
         */
        struct MANUFACTURER
        {
            using info_type   = CpuInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";       ///< WMI field for the CPU manufacturer.
        };

        /**
         * @struct PHYSICALCORES
         * @brief Encapsulates the NumberOfCores property of Win32_Processor.
         */
        struct PHYSICALCORES
        {
            using info_type   = CpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"NumberOfCores";      ///< WMI field for the number of physical cores.
        };

        /**
         * @struct LOGICALCORES
         * @brief Encapsulates the NumberOfLogicalProcessors property of Win32_Processor.
         */
        struct LOGICALCORES
        {
            using info_type   = CpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";              ///< WMI class name.
            static const inline std::wstring wmi_field = L"NumberOfLogicalProcessors";    ///< WMI field for the number of logical cores.
        };

        /**
         * @struct CLOCKSPEED
         * @brief Encapsulates the MaxClockSpeed property of Win32_Processor.
         */
        struct CLOCKSPEED
        {
            using info_type   = CpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"MaxClockSpeed";      ///< WMI field for the maximum clock speed.
        };

        /**
         * @struct L2CACHESIZE
         * @brief Encapsulates the L2CacheSize property of Win32_Processor.
         */
        struct L2CACHESIZE
        {
            using info_type   = CpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"L2CacheSize";        ///< WMI field for the L2 cache size.
        };

        /**
         * @struct L3CACHESIZE
         * @brief Encapsulates the L3CacheSize property of Win32_Processor.
         */
        struct L3CACHESIZE
        {
            using info_type   = CpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"L3CacheSize";        ///< WMI field for the L3 cache size.
        };
    };
}    // namespace hwinfo::WMI
