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
            // _____________________________________________________________________________________________________________________
            static std::string getVendor()
            {
                auto result = s_wmi.query< std::string >("Win32_PhysicalMemory", "Manufacturer");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            // _____________________________________________________________________________________________________________________
            static std::string getName()
            {
                auto result = s_wmi.query< std::string >("Win32_PhysicalMemory", "Name");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            // _____________________________________________________________________________________________________________________
            static std::string getModel()
            {
                auto result = s_wmi.query< std::string >("Win32_PhysicalMemory", "PartNumber");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            // _____________________________________________________________________________________________________________________
            static std::string getSerialNumber()
            {
                auto result = s_wmi.query< std::string >("Win32_PhysicalMemory", "SerialNumber");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            // _____________________________________________________________________________________________________________________
            static int64_t getTotalSize_Bytes()
            {
                auto result = s_wmi.query< std::string >("Win32_PhysicalMemory", "Capacity");
                if (result.empty()) return -1;

                return std::accumulate(result.begin(), result.end(), int64_t(0), [](int64_t sum, const std::string& val) {
                    return sum + std::stoll(val);
                });
            }

            // _____________________________________________________________________________________________________________________
            static int64_t getFreeMemory()
            {
                auto result = s_wmi.query< std::string >("CIM_OperatingSystem", "FreePhysicalMemory");
                if (result.empty()) return -1;
                return std::stoll(result.front()) * 1024;
            }

            static WMI::WMIInterface s_wmi;
        };

        WMI::WMIInterface RAMWin::s_wmi {};

    }    // namespace detail

    using RAM = detail::RAMWin;

}    // namespace hwinfo
