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

        struct BatteryData
        {
            std::string name {};
            std::string type {};
            std::string technology {};
            std::string status {};
            std::string health {};
            uint32_t    capacity { 0 };
        };

        std::vector< BatteryData > loadBatteryData()
        {
            utils::WMIWrapper wmi {};

            const std::wstring query_string(
                L"SELECT Name, Description, Chemistry, BatteryStatus, Status, FullChargeCapacity FROM Win32_Battery");
            bool success = wmi.execute_query(query_string);
            if (!success) return {};

            std::vector< BatteryData > batteries;

            ULONG             u_return = 0;
            IWbemClassObject* obj      = nullptr;
            int               cpu_id   = 0;
            while (wmi.m_enumerator) {
                wmi.m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                if (!u_return) break;

                BatteryData battery;
                //                battery._id = cpu_id++;
                VARIANT vt_prop;

                obj->Get(L"Name", 0, &vt_prop, nullptr, nullptr);
                battery.name = utils::wstring_to_std_string(vt_prop.bstrVal);

                obj->Get(L"Description", 0, &vt_prop, nullptr, nullptr);
                battery.type = utils::wstring_to_std_string(vt_prop.bstrVal);

                obj->Get(L"Chemistry", 0, &vt_prop, nullptr, nullptr);
                battery.technology = std::to_string(vt_prop.iVal);

                obj->Get(L"BatteryStatus", 0, &vt_prop, nullptr, nullptr);
                battery.status = std::to_string(vt_prop.iVal);

                obj->Get(L"Status", 0, &vt_prop, nullptr, nullptr);
                battery.health = utils::wstring_to_std_string(vt_prop.bstrVal);

                obj->Get(L"FullChargeCapacity", 0, &vt_prop, nullptr, nullptr);
                battery.capacity = vt_prop.uintVal;

                VariantClear(&vt_prop);
                obj->Release();
                batteries.push_back(std::move(battery));
            }
            return batteries;
        }

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
                std::vector< BatteryWin > batteries;
                auto                      res = s_wmi.query< std::string >("Win32_Battery", "Name");
                if (res.empty() || res.front().empty()) return {};

                int8_t counter = 0;
                for (const auto& v : res) {
                    batteries.emplace_back(counter++);
                    batteries.back()._model = v;
                }
                res.clear();

                auto temp = loadBatteryData();

                return batteries;
            }

            static utils::WMIWrapper s_wmi;
        };

        utils::WMIWrapper BatteryWin::s_wmi {};
    }    // namespace detail

    using Battery = detail::BatteryWin;

    std::vector< Battery > getBatteries() { return Battery::getAllBatteries(); }

}    // namespace hwinfo
