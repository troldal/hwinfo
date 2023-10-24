// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

#pragma once

#include "../base/ramBase.hpp"

#    include "../ram.hpp"
#    include "../utils/stringutils.hpp"
#include <unistd.h>

#include <fstream>
#include <string>
#include <vector>

namespace hwinfo {
    namespace detail
    {

        // _____________________________________________________________________________________________________________________
        struct MemInfo
        {
            int64_t total { -1 };
            int64_t free { -1 };
            int64_t available { -1 };
        };

        void get_from_sysconf(MemInfo& mi)
        {
            int64_t pages           = sysconf(_SC_PHYS_PAGES);
            int64_t available_pages = sysconf(_SC_AVPHYS_PAGES);
            int64_t page_size       = sysconf(_SC_PAGESIZE);
            if (pages > 0 && page_size > 0) {
                mi.total = pages * page_size;
            }
            if (available_pages > 0 && page_size > 0) {
                mi.available = available_pages * page_size;
            }
        }

        void set_value(std::string& line, int64_t* dst)
        {
            auto split_line = utils::split(line, ":");
            if (split_line.size() == 2) {
                auto& value = split_line[1];
                utils::strip(value);
                auto space = value.find(' ');
                if (space != std::string::npos) {
                    auto a = std::string(value.begin(), value.begin() + static_cast< int64_t >(space));
                    *dst   = (std::stoll(a) * 1024);
                    int i  = 0;
                }
            }
        }

        MemInfo parse_meminfo()
        {
            MemInfo       mi;
            std::ifstream f_meminfo("/proc/meminfo");
            if (!f_meminfo) {
                get_from_sysconf(mi);
            }
            else {
                while (mi.total == -1 || mi.available == -1 || mi.free == -1) {
                    std::string line;
                    if (!std::getline(f_meminfo, line)) {
                        if (mi.total == -1 || mi.available == -1) {
                            get_from_sysconf(mi);
                        }
                        return mi;
                    }
                    if (utils::starts_with(line, "MemTotal")) {
                        set_value(line, &mi.total);
                    }
                    else if (utils::starts_with(line, "MemFree")) {
                        set_value(line, &mi.free);
                    }
                    else if (utils::starts_with(line, "MemAvailable")) {
                        set_value(line, &mi.available);
                    }
                }
            }
            return mi;
        }

        class RAMLinux : public RAMBase< RAMLinux >
        {
            using BASE = RAMBase< RAMLinux >;
            friend BASE;

        public:
            RAMLinux()
            {
                BASE::_name            = "<unknown>";
                BASE::_vendor          = "<unknown>";
                BASE::_serialNumber    = "<unknown>";
                BASE::_model           = "<unknown>";
                auto meminfo           = parse_meminfo();
                BASE::_total_Bytes     = meminfo.total;
                BASE::_free_Bytes      = meminfo.free;
                BASE::_available_Bytes = meminfo.available;
            }

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
            std::string getModel() const
            {
                return BASE::_model;
            }

            [[nodiscard]]
            std::string getSerialNumber() const
            {
                return BASE::_serialNumber;
            }

            [[nodiscard]]
            int64_t getTotalMemory() const
            {
                return BASE::_total_Bytes;
            }

            [[nodiscard]]
            int64_t getFreeMemory() const
            {
                return BASE::_free_Bytes;
            }

            [[nodiscard]]
            int64_t getAvailableMemory() const
            {
                return BASE::_available_Bytes;
            }
        };

    }    // namespace detail

    using RAM = detail::RAMLinux;

}  // namespace hwinfo
