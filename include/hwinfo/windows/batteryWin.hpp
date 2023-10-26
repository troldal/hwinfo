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

#include "../base/batteryBase.hpp"
#include "utils/WMIWrapper.hpp"

namespace hwinfo
{

    namespace detail
    {
        class BatteryWin : public BatteryBase< BatteryWin >
        {
            using BASE = BatteryBase< BatteryWin >;

            friend BASE;

        public:
            explicit BatteryWin(int8_t id = 0)
                : BASE(id)
            {}

        private:
            [[nodiscard]]
            std::string getVendor() const
            {
                return "<unknown>";
            }

            [[nodiscard]]
            std::string getModel() const
            {
                return BASE::_model;
            }

            [[nodiscard]]
            std::string getSerialNumber() const
            {
                return "<unknown>";
            }

            [[nodiscard]]
            std::string getTechnology() const
            {
                return "<unknown>";
            }

            [[nodiscard]]
            uint32_t getEnergyFull() const
            {
                return 0;
            }

            [[nodiscard]]
            uint32_t getEnergyNow() const
            {
                return 0;
            }

            [[nodiscard]]
            bool getCharging() const
            {
                return false;
            }

            [[nodiscard]]
            bool getDischarging() const
            {
                return false;
            }

            static std::vector< BatteryWin > getAllBatteries_impl()
            {
                std::vector< BatteryWin >     batteries;
                // std::vector< const wchar_t* > res {};
                auto wmi = utils::WMIWrapper();
                auto res = wmi.query< std::string >(L"Win32_Battery", L"Name");
                // wmi::queryWMI("Win32_Battery", "Name", res);
                if (res.empty() || res.front().empty()) {
                    return {};
                }
                int8_t counter = 0;
                for (const auto& v : res) {
                    // std::wstring tmp(v);
                    batteries.emplace_back(counter++);
                    batteries.back()._model = v;    // utils::wstring_to_std_string(tmp);
                }
                res.clear();
                return batteries;
            }
        };
    }

    using Battery = detail::BatteryWin;

}    // namespace hwinfo
