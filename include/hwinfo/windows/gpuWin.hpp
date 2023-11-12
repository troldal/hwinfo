/*

    88        88  I8,        8        ,8I  88                 ad88
    88        88  `8b       d8b       d8'  88                d8"
    88        88   "8,     ,8"8,     ,8"   88                88
    88aaaaaaaa88    Y8     8P Y8     8P    88  8b,dPPYba,  MM88MMM  ,adPPYba,
    88""""""""88    `8b   d8' `8b   d8'    88  88P'   `"8a   88    a8"     "8a
    88        88     `8a a8'   `8a a8'     88  88       88   88    8b       d8
    88        88      `8a8'     `8a8'      88  88       88   88    "8a,   ,a8"
    88        88       `8'       `8'       88  88       88   88     `"YbbdP"'

    Based on the work of:
    Leon Freist <freist@informatik.uni-freiburg.de>

    Copyright © 2022 Leon Freist
    Copyright © 2023 Kenneth Troldal Balslev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the “Software”), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished
    to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @file GPUWin.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the GPUWin class, a Windows-specific implementation for GPU information retrieval.
 */

#pragma once

#include "../base/gpuBase.hpp"
#include "utils/WMIInterface.hpp"

#ifdef USE_OCL
#    include <missocl/opencl.h>
#endif

namespace hwinfo
{

    namespace detail
    {

        /**
         * @class GPUWin
         * @brief Windows-specific implementation for retrieving GPU information.
         *
         * This class inherits from GPUBase and implements GPU information retrieval for the Windows operating system.
         * It utilizes the WMI interface for querying system information and, optionally, OpenCL for additional GPU details.
         */
        class GPUWin : public GPUBase< GPUWin >
        {
            using BASE = GPUBase< GPUWin >;    ///< Alias for the base class type.
            friend BASE;                       ///< Make the base class a friend to allow it to access private members.

            /**
             * @brief Default constructor is private to ensure instances are only created through the factory method.
             */
            GPUWin() = default;

            /**
             * @brief Retrieves information about all GPUs on the system.
             * @return A GPUWin instance populated with information about all available GPUs.
             */
            [[nodiscard]]
            static GPUWin getAllGPUs()
            {
                GPUWin gpus;    // Create an instance to store GPU information.

                using namespace WMI;

                // Query GPU information using the WMI interface.
                auto info = wmiInterface.query< GpuInfo::NAME, GpuInfo::ADAPTERCOMPAT, GpuInfo::DRIVERVER, GpuInfo::ADAPTERRAM >();

                if (info.empty()) return {};    // If no GPUs are found, return an empty instance.

                // Iterate over the queried GPU information and add each GPU to the instance.
                for (const auto& gpu : info) {
                    auto item = BASE::GpuItem {};    // Create a GpuItem to store information about a single GPU.

                    // Populate the GpuItem with information from the query.
                    item.name          = std::get< 0 >(gpu);
                    item.vendor        = std::get< 1 >(gpu);
                    item.driverVersion = std::get< 2 >(gpu);
                    item.memory        = std::get< 3 >(gpu);

                    gpus.addItem(item);    // Add the GpuItem to the instance.
                }

#ifdef USE_OCL

                auto cl_gpus = mcl::DeviceManager::get_list< mcl::Filter::GPU >();
                for (auto& gpu : gpus) {
                    for (auto* cl_gpu : cl_gpus) {
                        if (cl_gpu->name() == gpu.name()) {
                            gpu._driverVersion = cl_gpu->driver_version();
                            gpu._frequency_MHz = static_cast< int64_t >(cl_gpu->clock_frequency_MHz());
                            gpu._num_cores     = static_cast< int >(cl_gpu->cores());
                            gpu._memory_Bytes  = static_cast< int64_t >(cl_gpu->memory_Bytes());
                            break;
                        }
                    }
                }
#endif    // USE_OCL
                return gpus;    // Return the instance populated with GPU information.
            }

            inline static WMI::WMIInterface wmiInterface {};    ///< Static instance of the WMI interface for querying system information.
        };

        //        WMI::WMIInterface GPUWin::wmiInterface {};    ///< Definition of the static WMI interface instance.

    }    // namespace detail

    using GPU = detail::GPUWin;    ///< Typedef for easier access to the GPUWin class outside of the detail namespace.

}    // namespace hwinfo
