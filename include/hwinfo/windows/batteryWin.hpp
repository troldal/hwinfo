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
#include "utils/WMIInterface.hpp"

namespace hwinfo
{

    namespace detail
    {
        class BatteryWin : public BatteryBase< BatteryWin >
        {
            using BASE = BatteryBase< BatteryWin >;

            friend BASE;

        public:
            explicit BatteryWin() = default;

        private:
            [[nodiscard]]
            uint32_t getCapacity() const    // NOLINT
            {
                using namespace WMI;
                auto res = wmiInterface.queryValue< BatteryInfo::CAPACITY >();
                if (res.empty()) return {};

                return res.front();
            }

            [[nodiscard]]
            std::string getStatus() const    // NOLINT
            {
                using namespace WMI;
                auto res = wmiInterface.queryValue< BatteryInfo::STATUS >();
                if (res.empty()) return {};

                return to_string(res.front());
            }

            static std::vector< BatteryWin > getAllBatteries_impl()
            {
                std::vector< BatteryWin > batteries;

                using namespace WMI;
                auto info = wmiInterface.queryRecord< BatteryInfo::NAME, BatteryInfo::CHEMISTRY, BatteryInfo::HEALTH >();

                auto& models    = std::get< 0 >(info);
                auto& chemistry = std::get< 1 >(info);
                auto& health    = std::get< 2 >(info);

                batteries.resize(models.size());

                auto modelIt     = models.begin();
                auto chemistryIt = chemistry.begin();
                auto healthIt    = health.begin();
                auto batteryIt   = batteries.begin();

                while (modelIt != models.end() and chemistryIt != chemistry.end() and healthIt != health.end()) {
                    batteryIt->_model      = *modelIt;
                    batteryIt->_technology = to_string(*chemistryIt);
                    batteryIt->_health     = *healthIt;

                    ++modelIt;
                    ++chemistryIt;
                    ++healthIt;
                    ++batteryIt;
                }

                return batteries;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface BatteryWin::wmiInterface {};
    }    // namespace detail

    using Battery = detail::BatteryWin;

}    // namespace hwinfo
