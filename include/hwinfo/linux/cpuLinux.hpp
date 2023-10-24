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
#include "../utils/stringutils.hpp"
#include "utils/filesystem.hpp"

#include <cmath>
#include <map>
#include <thread>

namespace hwinfo
{

    namespace detail
    {

        class CPULinux : public CPUBase< CPULinux >
        {
            using BASE = CPUBase< CPULinux >;
            friend BASE;

            struct Jiffies
            {
                Jiffies()
                {
                    working = -1;
                    all     = -1;
                }

                Jiffies(const int64_t& _all, const int64_t& _working)
                {
                    all     = _all;
                    working = _working;
                }

                int64_t working { -1 };
                int64_t all { -1 };
            };

        public:
            CPULinux() = default;

        private:
            [[nodiscard]]
            int getId() const
            {
                return BASE::_id;
            }

            [[nodiscard]]
            const std::string& getModelName() const
            {
                return BASE::_modelName;
            }

            [[nodiscard]]
            const std::string& getVendor() const
            {
                return BASE::_vendor;
            }

            [[nodiscard]]
            int64_t getL1CacheSize() const
            {
                return BASE::_L1CacheSize_Bytes;
            }

            [[nodiscard]]
            int64_t getL2CacheSize() const
            {
                return BASE::_L2CacheSize_Bytes;
            }

            [[nodiscard]]
            int64_t getL3CacheSize() const
            {
                return BASE::_L3CacheSize_Bytes;
            }

            [[nodiscard]]
            int getNumPhysicalCores() const
            {
                return BASE::_numPhysicalCores;
            }

            [[nodiscard]]
            int getNumLogicalCores() const
            {
                return BASE::_numLogicalCores;
            }

            [[nodiscard]]
            int64_t getMaxClockSpeed() const
            {
                return BASE::_maxClockSpeed_MHz;
            }

            [[nodiscard]]
            int64_t getRegularClockSpeed() const
            {
                return BASE::_regularClockSpeed_MHz;
            }

            [[nodiscard]]
            int64_t getMinClockSpeed_MHz(int core_id)
            {
                int64_t Hz = filesystem::get_specs_by_file_path("/sys/devices/system/cpu/cpu" + std::to_string(core_id) +
                                                                "/cpufreq/scaling_min_freq");
                if (Hz > -1) {
                    return Hz / 1000;
                }

                return -1;
            }

            [[nodiscard]]
            int64_t getCurrentClockSpeed(int thread_id) const
            {
                //            auto data =
                //                utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation",
                //                L"PercentProcessorPerformance");
                //            if (data.empty()) {
                //                return -1;
                //            }
                //            double performance = std::stod(data[thread_id]) / 100;
                //            return static_cast< int64_t >(static_cast< double >(_maxClockSpeed_MHz) * performance);

                return -1;
            }

            [[nodiscard]]
            std::vector< int64_t > getCurrentClockSpeed() const
            {
                std::vector< int64_t > res;
                res.reserve(numLogicalCores());
                for (int core_id = 0; /* breaks, if i is no valid cpu id */; ++core_id) {
                    int64_t frequency_Hz = filesystem::get_specs_by_file_path("/sys/devices/system/cpu/cpu" + std::to_string(core_id) +
                                                                              "/cpufreq/scaling_cur_freq");
                    if (frequency_Hz == -1) {
                        break;
                    }
                    res.push_back(frequency_Hz / 1000);
                }

                return res;
            }

            [[nodiscard]]
            double getCurrentUtilisation() const
            {
                init_jiffies();
                // TODO: Leon Freist a socket max num and a socket id inside the CPU could make it work with all sockets
                //       I will not support it because I only have a 1 socket target device
                static Jiffies last = Jiffies();

                Jiffies current = get_jiffies(0);

                auto total_over_period = static_cast< double >(current.all - last.all);
                auto work_over_period  = static_cast< double >(current.working - last.working);

                last = current;

                const double utilization = work_over_period / total_over_period;
                if (utilization < 0 || utilization > 1 || std::isnan(utilization)) {
                    return -1.0;
                }
                return utilization;
            }

            [[nodiscard]]
            double getThreadUtilisation(int thread_id) const
            {
                init_jiffies();
                // TODO: Leon Freist a socket max num and a socket id inside the CPU could make it work with all sockets
                //       I will not support it because I only have a 1 socket target device
                static std::vector< Jiffies > last(0);
                if (last.empty()) {
                    last.resize(_numLogicalCores);
                }

                Jiffies current = get_jiffies(thread_id + 1);    // thread_index works only with 1 socket right now

                auto total_over_period = static_cast< double >(current.all - last[thread_id].all);
                auto work_over_period  = static_cast< double >(current.working - last[thread_id].working);

                last[thread_id] = current;

                const double percentage = work_over_period / total_over_period;
                if (percentage < 0 || percentage > 100 || std::isnan(percentage)) {
                    return -1.0;
                }
                return percentage;
            }

            [[nodiscard]]
            std::vector< double > getThreadsUtilisation() const
            {
                std::vector< double > thread_utility(BASE::_numLogicalCores);
                for (int thread_idx = 0; thread_idx < BASE::_numLogicalCores; ++thread_idx) {
                    thread_utility[thread_idx] = threadUtilisation(thread_idx);
                }
                return thread_utility;
            }

            [[nodiscard]]
            const std::vector< std::string >& getFlags() const
            {
                return BASE::_flags;
            }

        private:
            [[nodiscard]]
            static int64_t getMaxClockSpeed_MHz(int core_id)
            {
                int64_t Hz = filesystem::get_specs_by_file_path("/sys/devices/system/cpu/cpu" + std::to_string(core_id) +
                                                                "/cpufreq/scaling_max_freq");
                if (Hz > -1) {
                    return Hz / 1000;
                }

                return -1;
            }

            [[nodiscard]]
            static int64_t getRegularClockSpeed_MHz(int core_id)
            {
                int64_t Hz =
                    filesystem::get_specs_by_file_path("/sys/devices/system/cpu/cpu" + std::to_string(core_id) + "/cpufreq/base_frequency");
                if (Hz > -1) {
                    return Hz / 1000;
                }

                return -1;
            }

            mutable bool _jiffies_initialized = false;

            Jiffies get_jiffies(int index) const
            {
                // std::string text = "cpu  349585 0 30513 875546 0 935 0 0 0 0";

                std::ifstream filestat("/proc/stat");
                if (!filestat.is_open()) {
                    return {};
                }

                for (int i = 0; i < index; ++i) {
                    if (!filestat.ignore(std::numeric_limits< std::streamsize >::max(), '\n')) {
                        break;
                    }
                }
                std::string line;
                std::getline(filestat, line);

                std::istringstream         iss(line);
                std::vector< std::string > results(std::istream_iterator< std::string > { iss }, std::istream_iterator< std::string >());

                const int jiffies_0 = std::stoi(results[1]);
                const int jiffies_1 = std::stoi(results[2]);
                const int jiffies_2 = std::stoi(results[3]);
                const int jiffies_3 = std::stoi(results[4]);
                const int jiffies_4 = std::stoi(results[5]);
                const int jiffies_5 = std::stoi(results[6]);
                const int jiffies_6 = std::stoi(results[7]);
                const int jiffies_7 = std::stoi(results[8]);
                const int jiffies_8 = std::stoi(results[9]);
                const int jiffies_9 = std::stoi(results[10]);

                int64_t all =
                    jiffies_0 + jiffies_1 + jiffies_2 + jiffies_3 + jiffies_4 + jiffies_5 + jiffies_6 + jiffies_7 + jiffies_8 + jiffies_9;
                int64_t working = jiffies_0 + jiffies_1 + jiffies_2;

                return { all, working };
            }

            void init_jiffies() const
            {
                if (!_jiffies_initialized) {
                    // Sleep 1 sec just for the start cause the usage needs to have a delta value which is depending on the unix file
                    // read it's just for the init, you don't need to wait if the delta is already created ...
                    std::this_thread::sleep_for(std::chrono::duration< double >(1));
                    _jiffies_initialized = true;
                }
            }

            static std::vector< CPULinux > getAllCPUs_impl()
            {
                std::vector< CPULinux > cpus;

                std::ifstream cpuinfo("/proc/cpuinfo");
                if (!cpuinfo.is_open()) {
                    return {};
                }
                std::string file((std::istreambuf_iterator< char >(cpuinfo)), (std::istreambuf_iterator< char >()));
                cpuinfo.close();
                auto                                             cpu_blocks_string = utils::split(file, "\n\n");
                std::map< const std::string, const std::string > cpu_block;
                int                                              physical_id = -1;
                bool                                             next_add    = false;
                for (const auto& block : cpu_blocks_string) {
                    CPULinux cpu;
                    auto     lines = utils::split(block, '\n');
                    for (auto& line : lines) {
                        auto line_pairs = utils::split(line, ":");
                        if (line_pairs.size() < 2) {
                            continue;
                        }
                        auto name  = line_pairs[0];
                        auto value = line_pairs[1];
                        utils::strip(name);
                        utils::strip(value);
                        if (name == "vendor_id") {
                            cpu._vendor = value;
                        }
                        else if (name == "model name") {
                            cpu._modelName = value;
                        }
                        else if (name == "cache size") {
                            cpu._L3CacheSize_Bytes = std::stoi(utils::split(value, " ")[0]) * 1024;
                        }
                        else if (name == "siblings") {
                            cpu._numLogicalCores = std::stoi(value);
                        }
                        else if (name == "cpu cores") {
                            cpu._numPhysicalCores = std::stoi(value);
                        }
                        else if (name == "flags") {
                            cpu._flags = utils::split(value, " ");
                        }
                        else if (name == "physical id") {
                            int tmp_phys_id = std::stoi(value);
                            if (physical_id == tmp_phys_id) {
                                continue;
                            }
                            cpu._id  = tmp_phys_id;
                            next_add = true;
                        }
                    }
                    if (next_add) {
                        cpu._maxClockSpeed_MHz     = getMaxClockSpeed_MHz(cpu._id);
                        cpu._regularClockSpeed_MHz = getRegularClockSpeed_MHz(cpu._id);
                        next_add                   = false;
                        physical_id++;
                        cpus.push_back(std::move(cpu));
                    }
                }
                return cpus;
            }
        };

    }

    using CPU = detail::CPULinux;

}    // namespace hwinfo
