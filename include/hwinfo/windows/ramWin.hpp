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

#include <numeric>
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

        public:
            RAMWin()
            {
                BASE::_name            = getName();
                BASE::_vendor          = getVendor();
                BASE::_serialNumber    = getSerialNumber();
                BASE::_model           = getModel();
                BASE::_total_Bytes     = getTotalSize_Bytes();
                BASE::_free_Bytes      = getFreeMemory();
                BASE::_available_Bytes = getFreeMemory();
            }

        private:
            static std::string getVendor()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< RamInfo::MANUFACTURER >();
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            static std::string getName()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< RamInfo::NAME >();
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            static std::string getModel()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< RamInfo::PARTNUMBER >();
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            static std::string getSerialNumber()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< RamInfo::SERIALNUMBER >();
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            static uint64_t getTotalSize_Bytes()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< RamInfo::CAPACITY >();
                if (result.empty()) return -1;

                return std::accumulate(result.begin(), result.end(), uint64_t(0), [](uint64_t sum, const std::string& val) {
                    return sum + std::stoll(val);
                });
            }

            static int64_t getFreeMemory()
            {
                using namespace WMI;
                auto result = wmiInterface.queryValue< OSInfo::FREEMEMORY >();
                if (result.empty()) return -1;
                return std::stoll(result.front()) * 1024;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface RAMWin::wmiInterface {};

    }    // namespace detail

    using RAM = detail::RAMWin;

}    // namespace hwinfo
