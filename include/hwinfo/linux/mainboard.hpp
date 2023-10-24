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

#include "../base/mainboard.hpp"

namespace hwinfo
{

    namespace detail
    {

        class MainBoardLinux : public MainBoardBase< MainBoardLinux >
        {
            using BASE = MainBoardBase< MainBoardLinux >;
            friend BASE;

        public:
            MainBoardLinux()
            {
                BASE::_vendor       = get_dmi_by_name("board_vendor");
                BASE::_name         = get_dmi_by_name("board_name");
                BASE::_version      = get_dmi_by_name("board_version");
                BASE::_serialNumber = get_dmi_by_name("board_serial");
            }

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
            std::string getVersion() const
            {
                return BASE::_version;
            }

            [[nodiscard]]
            std::string getSerialNumber() const
            {
                return BASE::_serialNumber;
            }

            static std::string get_dmi_by_name(const std::string& name)
            {
                std::string                value;
                std::vector< std::string > candidates = { "/sys/devices/virtual/dmi/", "/sys/class/dmi/" };
                for (const auto& path : candidates) {
                    std::string full_path(path);
                    full_path.append("id/");
                    full_path.append(name);
                    std::ifstream f(full_path);
                    if (f) {
                        getline(f, value);
                        if (!value.empty()) {
                            return value;
                        }
                    }
                }
                return "<unknown>";
            }
        };

    }    // namespace detail

    using MainBoard = detail::MainBoardLinux;

}    // namespace hwinfo
