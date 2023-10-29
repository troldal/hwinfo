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
    struct CpuInfo
    {
        /**
         * @struct NAME
         * @brief
         */
        struct NAME
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";               ///< WMI field for the battery name.
        };

        /**
         * @struct MANUFACTURER
         * @brief
         */
        struct MANUFACTURER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Manufacturer";       ///< WMI field for the battery description.
        };

        /**
         * @struct PHYSICALCORES
         * @brief
         */
        struct PHYSICALCORES
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"NumberOfCores";      ///< WMI field for the battery chemistry.
        };

        /**
         * @struct LOGICALCORES
         * @brief
         */
        struct LOGICALCORES
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";              ///< WMI class name.
            static const inline std::wstring wmi_field = L"NumberOfLogicalProcessors";    ///< WMI field for the battery chemistry.
        };

        /**
         * @struct CLOCKSPEED
         * @brief
         */
        struct CLOCKSPEED
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"MaxClockSpeed";      ///< WMI field for the battery status.
        };

        /**
         * @struct L2CACHESIZE
         * @brief
         */
        struct L2CACHESIZE
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"L2CacheSize";        ///< WMI field for the battery status.
        };

        /**
         * @struct L3CACHESIZE
         * @brief
         */
        struct L3CACHESIZE
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Processor";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"L3CacheSize";        ///< WMI field for the battery status.
        };
    };
}    // namespace hwinfo::WMI
