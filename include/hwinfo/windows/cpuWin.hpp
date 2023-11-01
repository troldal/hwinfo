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

#include "../base/cpuBase.hpp"
#include "utils/WMIInterface.hpp"

#include <numeric>

namespace hwinfo
{
    namespace detail
    {

        class CPUWin : public CPUBase< CPUWin >
        {
            using BASE = CPUBase< CPUWin >;
            friend BASE;

            CPUWin() = default;

        private:
            [[nodiscard]]
            int64_t getCurrentClockSpeed(int thread_id) const
            {
                using namespace WMI;
                auto info = wmiInterface.query< PerfInfo::PROCESSORPERFORMANCE >();

                auto clockSpeed = _items.front().maxClockSpeed;

                if (info.empty()) return -1;
                else
                    return static_cast< int64_t >(static_cast< double >(clockSpeed) * std::stod(std::get< 0 >(info[thread_id])) / 100.0);
            }

            [[nodiscard]]
            std::vector< int64_t > getCurrentClockSpeed() const
            {
                std::vector< int64_t > result;
                result.reserve(logicalCoreCount());

                using namespace WMI;
                auto info = wmiInterface.query< PerfInfo::PROCESSORPERFORMANCE >();

                auto clockSpeed = _items.front().maxClockSpeed;

                if (info.empty()) result.assign(logicalCoreCount(), -1);
                else
                    for (auto& v : info)
                        result.push_back(static_cast< int64_t >(static_cast< double >(clockSpeed) * std::stod(std::get< 0 >(v)) / 100.0));

                return result;
            }

            [[nodiscard]]
            double getCurrentUtilisation() const
            {
                auto tutils = getThreadsUtilisation();
                if (tutils.empty()) return -1.0;
                else
                    return std::accumulate(tutils.begin(), tutils.end(), 0.0) / tutils.size();
            }

            [[nodiscard]]
            double getThreadUtilisation(int thread_id) const    // NOLINT
            {
                using namespace WMI;
                auto info = wmiInterface.query< PerfInfo::PROCESSORUTILITY >();

                if (info.empty()) return -1.f;
                else
                    return std::stod(std::get< 0 >(info[thread_id])) / 100.f;
            }

            [[nodiscard]]
            std::vector< double > getThreadsUtilisation() const
            {
                std::vector< double > thread_utility;
                thread_utility.reserve(logicalCoreCount());

                using namespace WMI;
                auto info = wmiInterface.query< PerfInfo::PROCESSORUTILITY >();

                if (info.empty()) thread_utility.assign(logicalCoreCount(), -1.0);
                else
                    for (const auto& cpu : info) thread_utility.push_back(std::stod(std::get< 0 >(cpu)) / 100.f);

                return thread_utility;
            }

            [[nodiscard]]
            static CPUWin getAllCPUs()
            {
                CPUWin cpus;

                using namespace WMI;
                auto info = wmiInterface.query< CpuInfo::NAME,
                                                CpuInfo::MANUFACTURER,
                                                CpuInfo::PHYSICALCORES,
                                                CpuInfo::LOGICALCORES,
                                                CpuInfo::CLOCKSPEED,
                                                CpuInfo::L2CACHESIZE,
                                                CpuInfo::L3CACHESIZE >();

                for (const auto& cpu : info) {
                    auto item = BASE::CpuItem {};

                    item.modelName         = std::get< 0 >(cpu);
                    item.vendor            = std::get< 1 >(cpu);
                    item.numPhysicalCores  = std::get< 2 >(cpu);
                    item.numLogicalCores   = std::get< 3 >(cpu);
                    item.maxClockSpeed     = std::get< 4 >(cpu);
                    item.regularClockSpeed = std::get< 4 >(cpu);
                    item.L2CacheSize       = std::get< 5 >(cpu);
                    item.L3CacheSize       = std::get< 6 >(cpu);

                    cpus.addItem(item);
                }

                return cpus;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface CPUWin::wmiInterface {};

    }    // namespace detail

    using CPU = detail::CPUWin;

}    // namespace hwinfo
