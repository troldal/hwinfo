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

#include "../base/gpu.h"

namespace hwinfo
{

    namespace detail
    {

        class GPULinux : public GPUBase< GPULinux >
        {
            using BASE = GPUBase< GPULinux >;
            friend BASE;

        public:
            GPULinux() = default;

        private:
            [[nodiscard]]
            std::string getVendor() const
            {
                return BASE::_vendor;
            }

            [[nodiscard]]
            std::string getName() const
            {
                return BASE::_name;
            }

            [[nodiscard]]
            std::string getDriverVersion() const
            {
                return BASE::_driverVersion;
            }

            [[nodiscard]]
            int64_t getMemory() const
            {
                return BASE::_memory_Bytes;
            }

            [[nodiscard]]
            int64_t getFrequency() const
            {
                return BASE::_frequency_MHz;
            }

            [[nodiscard]]
            int getNumCores() const
            {
                return BASE::_num_cores;
            }

            [[nodiscard]]
            int getId() const
            {
                return BASE::_id;
            }

            static std::vector< GPULinux > getAllGPUs_impl()
            {
                std::vector< GPULinux > gpus {};
                PCIMapper               pci = PCI::getMapper();
                int                     id  = 0;
                while (true) {
                    GPULinux gpu;
                    gpu._id = id;
                    std::string path("/sys/class/drm/card" + std::to_string(id) + '/');
                    if (!filesystem::exists(path)) {
                        if (id > 2) {
                            break;
                        }
                        id++;
                        continue;
                    }
                    gpu._vendor_id = read_drm_by_path(path + "device/vendor");
                    gpu._device_id = read_drm_by_path(path + "device/device");
                    if (gpu._vendor_id.empty() || gpu._device_id.empty()) {
                        id++;
                        continue;
                    }
                    const PCIVendor& vendor = pci[gpu._vendor_id];
                    const PCIDevice  device = vendor[gpu._device_id];
                    gpu._vendor             = vendor.vendor_name;
                    gpu._name               = vendor[gpu._device_id].device_name;
                    auto frequencies        = get_frequencies(path);
                    gpu._frequency_MHz      = frequencies[2];
                    gpus.push_back(std::move(gpu));
                    id++;
                }
#ifdef USE_OCL
                auto cl_gpus = mcl::DeviceManager::get_list< mcl::Filter::GPU >();
                for (auto& gpu : gpus) {
                    for (auto* cl_gpu : cl_gpus) {
                        if (cl_gpu->name().find(gpu._device_id)) {
                            gpu._driverVersion = cl_gpu->driver_version();
                            gpu._frequency_MHz = static_cast< int64_t >(cl_gpu->clock_frequency_MHz());
                            gpu._num_cores     = static_cast< int >(cl_gpu->cores());
                            gpu._memory_Bytes  = static_cast< int64_t >(cl_gpu->memory_Bytes());
                        }
                    }
                }
#endif    // USE_OCL
                return gpus;
            }

            static std::string read_drm_by_path(const std::string& path)
            {
                std::ifstream f_drm(path);
                if (!f_drm) {
                    return "";
                }
                std::string ret;
                getline(f_drm, ret);
                return ret;
            }

            static std::vector< int > get_frequencies(const std::string drm_path)
            {
                // {min, current, max}
                std::vector< int > freqs(3);
                try {
                    freqs[0] = std::stoi(read_drm_by_path(drm_path + "gt_min_freq_mhz"));
                }
                catch (const std::invalid_argument& e) {
                    freqs[0] = -1;
                }
                try {
                    freqs[1] = std::stoi(read_drm_by_path(drm_path + "gt_cur_freq_mhz"));
                }
                catch (const std::invalid_argument& e) {
                    freqs[0] = -1;
                }
                try {
                    freqs[2] = std::stoi(read_drm_by_path(drm_path + "gt_max_freq_mhz"));
                }
                catch (const std::invalid_argument& e) {
                    freqs[0] = -1;
                }
                return freqs;
            }
        };

    }    // namespace detail

    using GPU = detail::GPULinux;

}    // namespace hwinfo
