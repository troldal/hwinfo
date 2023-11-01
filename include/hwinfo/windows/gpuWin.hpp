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

        class GPUWin : public GPUBase< GPUWin >
        {
            using BASE = GPUBase< GPUWin >;
            friend BASE;

            GPUWin() = default;

        private:
            [[nodiscard]]
            static GPUWin getAllGPUs()
            {
                GPUWin gpus;

                using namespace WMI;
                auto info = wmiInterface.query< GpuInfo::NAME, GpuInfo::ADAPTERCOMPAT, GpuInfo::DRIVERVER, GpuInfo::ADAPTERRAM >();

                if (info.empty()) return {};

                for (const auto& gpu : info) {
                    auto item = BASE::GpuItem {};

                    item.name          = std::get< 0 >(gpu);
                    item.vendor        = std::get< 1 >(gpu);
                    item.driverVersion = std::get< 2 >(gpu);
                    item.memory        = std::get< 3 >(gpu);

                    gpus.addItem(item);
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
                return gpus;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface GPUWin::wmiInterface {};

    }    // namespace detail

    using GPU = detail::GPUWin;

}    // namespace hwinfo
