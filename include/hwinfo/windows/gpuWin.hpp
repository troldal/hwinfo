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
#include "utils/WMIWrapper.hpp"

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

        public:
            GPUWin() = default;

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

            static std::vector< GPUWin > getAllGPUs_impl()
            {
                utils::WMI::WMIWrapper wmi {};
                const std::wstring query_string(L"SELECT Name, AdapterCompatibility, DriverVersion, AdapterRam "
                                                L"FROM WIN32_VideoController");
                bool               success = wmi.execute_query(query_string);
                if (!success) {
                    return {};
                }
                std::vector< GPUWin > gpus;

                ULONG             u_return = 0;
                IWbemClassObject* obj      = nullptr;
                int               gpu_id   = 0;
                while (wmi.m_enumerator) {
                    wmi.m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                    if (!u_return) {
                        break;
                    }
                    GPUWin gpu;
                    gpu._id = gpu_id++;
                    VARIANT vt_prop;
                    obj->Get(L"Name", 0, &vt_prop, nullptr, nullptr);
                    gpu._name = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"AdapterCompatibility", 0, &vt_prop, nullptr, nullptr);
                    gpu._vendor = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"DriverVersion", 0, &vt_prop, nullptr, nullptr);
                    gpu._driverVersion = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"AdapterRam", 0, &vt_prop, nullptr, nullptr);
                    gpu._memory_Bytes = vt_prop.intVal;
                    VariantClear(&vt_prop);
                    obj->Release();
                    gpus.push_back(std::move(gpu));
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
        };

    }    // namespace detail

    using GPU = detail::GPUWin;

}    // namespace hwinfo
