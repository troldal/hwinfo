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

#include "../base/diskBase.hpp"
#include "utils/WMIInterface.hpp"

namespace hwinfo
{

    namespace detail
    {

        class DiskWin : public DiskBase< DiskWin >
        {
            using BASE = DiskBase< DiskWin >;
            friend BASE;

        public:
            DiskWin() = default;

        private:
            static std::vector< DiskWin > getAllDisks_impl()
            {
                std::vector< DiskWin > disks;

                using namespace WMI;
                auto info = wmiInterface.queryRecord< DiskInfo::MODEL, DiskInfo::MANUFACTURER, DiskInfo::SERIALNUMBER, DiskInfo::SIZE >();

                for (const auto& disk : info) {
                    disks.emplace_back();
                    auto& processor = disks.back();

                    processor.m_model        = std::get< 0 >(disk);
                    processor.m_vendor       = std::get< 1 >(disk);
                    processor.m_serialNumber = std::get< 2 >(disk);
                    processor.m_size         = std::stoull(std::get< 3 >(disk));
                }

                return disks;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface DiskWin::wmiInterface {};

    }    // namespace detail

    using Disk = detail::DiskWin;

}    // namespace hwinfo
