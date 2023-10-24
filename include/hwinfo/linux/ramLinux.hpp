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

#include "../base/ramBase.hpp"

#include <unistd.h>

namespace hwinfo {
    namespace detail
    {
        class RAMLinux : public RAMBase< RAMLinux >
        {
            using BASE = RAMBase< RAMLinux >;
            friend BASE;

            struct MemInfo
            {
                int64_t total { -1 };
                int64_t free { -1 };
                int64_t available { -1 };
            };

            static void get_from_sysconf(MemInfo& mi)
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

            static void set_value(std::string& line, int64_t* dst)
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

            static MemInfo parse_meminfo()
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

        };

    }    // namespace detail

    using RAM = detail::RAMLinux;

}  // namespace hwinfo
