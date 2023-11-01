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
#include "utils/WMIInterface.hpp"

#include <string>
#include <vector>

namespace hwinfo
{
    namespace detail
    {

        class RAMWin : public RAMBase< RAMWin >
        {
            using BASE = RAMBase< RAMWin >;
            friend BASE;

            RAMWin() = default;

        private:
            [[nodiscard]]
            static RAMWin getAllRam()
            {
                RAMWin ram_blocks;

                using namespace WMI;
                auto info =
                    wmiInterface
                        .query< RamInfo::MANUFACTURER, RamInfo::NAME, RamInfo::PARTNUMBER, RamInfo::SERIALNUMBER, RamInfo::CAPACITY >();

                for (const auto& block : info) {
                    auto item = BASE::RamItem {};

                    item.vendor       = std::get< 0 >(block);
                    item.name         = std::get< 1 >(block);
                    item.model        = std::get< 2 >(block);
                    item.serialNumber = std::get< 3 >(block);
                    item.totalMem     = std::stoll(std::get< 4 >(block));

                    ram_blocks.addItem(item);
                }

                auto result         = wmiInterface.query< OSInfo::FREEMEMORY >();
                ram_blocks._freeRam = (result.empty()) ? 0 : std::stoll(std::get< 0 >(result.front())) * 1024;

                return ram_blocks;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface RAMWin::wmiInterface {};

    }    // namespace detail

    using RAM = detail::RAMWin;

}    // namespace hwinfo
