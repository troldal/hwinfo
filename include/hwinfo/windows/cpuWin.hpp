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

namespace hwinfo
{
    namespace detail
    {

        class CPUWin : public CPUBase< CPUWin >
        {
            using BASE = CPUBase< CPUWin >;
            friend BASE;

        public:
            CPUWin() = default;

        private:
            [[nodiscard]]
            int64_t getCurrentClockSpeed(int thread_id) const
            {
                auto data = wmiInterface.query< std::string >("Win32_PerfFormattedData_Counters_ProcessorInformation",
                                                              "PercentProcessorPerformance");
                if (data.empty()) return -1;

                double performance = std::stod(data[thread_id]) / 100;
                return static_cast< int64_t >(static_cast< double >(m_maxClockSpeed) * performance);
            }

            [[nodiscard]]
            std::vector< int64_t > getCurrentClockSpeed() const
            {
                std::vector< int64_t > result;
                result.reserve(m_numLogicalCores);
                auto data = wmiInterface.query< std::string >("Win32_PerfFormattedData_Counters_ProcessorInformation",
                                                              "PercentProcessorPerformance");
                if (data.empty()) {
                    result.resize(m_numLogicalCores, -1);
                    return result;
                }
                for (auto& v : data) {
                    double performance = std::stod(v) / 100;
                    result.push_back(static_cast< int64_t >(static_cast< double >(m_maxClockSpeed) * performance));
                }
                return result;
            }

            [[nodiscard]]
            double getCurrentUtilisation() const
            {
                //                std::vector< bstr_t > percentage {};
                //                const std::string&    query =
                //                    "Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name='" + std::to_string(0) + ",_Total'";
                //                wmi::queryWMI(query, "PercentProcessorUtility", percentage);
                //                if (percentage.empty()) {
                //                    return -1.0;
                //                }
                //
                //                const char* strValue = static_cast< const char* >(percentage[0]);
                //                return std::stod(strValue);

                std::string filter = "Name='" + std::to_string(0) + ",_Total'";

                auto data = wmiInterface.query< std::string >("Win32_PerfFormattedData_Counters_ProcessorInformation",
                                                              "PercentProcessorUtility",
                                                              filter);

                if (data.empty()) return -1.0;
                else
                    return std::stod(data.front());
            }

            [[nodiscard]]
            double getThreadUtilisation(int thread_id) const
            {
                //                auto data =
                //                    utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation",
                //                    L"PercentProcessorUtility");
                //                if (data.empty()) {
                //                    return -1.f;
                //                }
                //                std::string thread_value = data[thread_id];
                //                if (thread_value.empty()) {
                //                    return -1.f;
                //                }
                //                return std::stod(thread_value);

                auto data =
                    wmiInterface.query< std::string >("Win32_PerfFormattedData_Counters_ProcessorInformation", "PercentProcessorUtility");

                if (data.empty()) return -1.f;
                else
                    return std::stod(data[thread_id]);    // / 100.f;
            }

            [[nodiscard]]
            std::vector< double > getThreadsUtilisation() const
            {
                std::vector< double > thread_utility;
                thread_utility.reserve(m_numLogicalCores);

                auto data =
                    wmiInterface.query< std::string >("Win32_PerfFormattedData_Counters_ProcessorInformation", "PercentProcessorUtility");
                if (data.empty()) {
                    thread_utility.resize(m_numLogicalCores, -1.f);
                    return thread_utility;
                }
                for (const auto& v : data) {
                    if (v.empty()) thread_utility.push_back(-1.f);
                    else
                        thread_utility.push_back(std::stod(v) / 100.f);
                }
                return thread_utility;
            }

            [[nodiscard]]
            static std::vector< CPUWin > getAllCPUs_impl()
            {
                std::vector< CPUWin > cpus;

                using namespace WMI;
                auto info = wmiInterface.queryRecord< CpuInfo::NAME,
                                                      CpuInfo::MANUFACTURER,
                                                      CpuInfo::PHYSICALCORES,
                                                      CpuInfo::LOGICALCORES,
                                                      CpuInfo::CLOCKSPEED,
                                                      CpuInfo::L2CACHESIZE,
                                                      CpuInfo::L3CACHESIZE >();

                for (const auto& cpu : info) {
                    cpus.emplace_back();
                    auto& processor = cpus.back();

                    processor.m_modelName         = std::get< 0 >(cpu);
                    processor.m_vendor            = std::get< 1 >(cpu);
                    processor.m_numPhysicalCores  = std::get< 2 >(cpu);
                    processor.m_numLogicalCores   = std::get< 3 >(cpu);
                    processor.m_maxClockSpeed     = std::get< 4 >(cpu);
                    processor.m_regularClockSpeed = std::get< 4 >(cpu);
                    processor.m_L2CacheSize       = std::get< 5 >(cpu);
                    processor.m_L3CacheSize       = std::get< 6 >(cpu);
                }

                return cpus;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface CPUWin::wmiInterface {};

    }    // namespace detail

    using CPU = detail::CPUWin;

}    // namespace hwinfo
