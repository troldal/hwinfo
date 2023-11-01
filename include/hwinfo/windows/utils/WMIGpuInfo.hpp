//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct GpuInfo
     * @brief Represents GPU (Graphics Processing Unit) information obtained from WMI.
     */
    class GpuInfo
    {
        struct GpuInfoType
        {
        };

    public:
        /**
         * @struct NAME
         * @brief Encapsulates the Name property of Win32_VideoController.
         */
        struct NAME
        {
            using info_type   = GpuInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";                     ///< WMI field for the GPU name.
        };

        /**
         * @struct ADAPTERCOMPAT
         * @brief Encapsulates the AdapterCompatibility property of Win32_VideoController.
         */
        struct ADAPTERCOMPAT
        {
            using info_type   = GpuInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"AdapterCompatibility";     ///< WMI field for the GPU adapter compatibility.
        };

        /**
         * @struct DRIVERVER
         * @brief Encapsulates the DriverVersion property of Win32_VideoController.
         */
        struct DRIVERVER
        {
            using info_type   = GpuInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"DriverVersion";            ///< WMI field for the GPU driver version.
        };

        /**
         * @struct ADAPTERRAM
         * @brief Encapsulates the AdapterRAM property of Win32_VideoController.
         */
        struct ADAPTERRAM
        {
            using info_type   = GpuInfoType;
            using value_type  = uint32_t;    ///< Type of the value being used.
            using result_type = uint32_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_VideoController";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"AdapterRAM";               ///< WMI field for the size of GPU memory.
        };
    };
}    // namespace hwinfo::WMI
