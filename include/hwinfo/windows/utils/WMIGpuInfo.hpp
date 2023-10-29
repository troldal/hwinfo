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
    struct GpuInfo
    {
        /**
         * @struct NAME
         * @brief
         */
        struct NAME
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"WIN32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";                     ///< WMI field for the battery name.
        };

        /**
         * @struct ADAPTERCOMPAT
         * @brief
         */
        struct ADAPTERCOMPAT
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"WIN32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"AdapterCompatibility";     ///< WMI field for the battery description.
        };

        /**
         * @struct DRIVERVER
         * @brief
         */
        struct DRIVERVER
        {
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"WIN32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"DriverVersion";            ///< WMI field for the battery chemistry.
        };

        /**
         * @struct ADAPTERRAM
         * @brief
         */
        struct ADAPTERRAM
        {
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"WIN32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"AdapterRAM";               ///< WMI field for the battery chemistry.
        };
    };
}    // namespace hwinfo::WMI
