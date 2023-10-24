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


#include <fstream>
#include <filesystem>

#include "../base/battery.h"

namespace hwinfo {

    class BatteryLinux : public BatteryBase<BatteryLinux>
    {
        using BASE = BatteryBase<BatteryLinux>;
        friend BASE;

        static const std::string base_path;

    public:

        explicit BatteryLinux(int8_t id = 0) : BASE(id) {}

        static std::vector<BatteryLinux> getAllBatteries_impl() {
            std::vector<BatteryLinux> batteries;
            int8_t id = 0;
            while (std::filesystem::exists(base_path + "BAT" + std::to_string(id))) {
                batteries.emplace_back(id++);
            }
            return batteries;
        }

    private:

        [[nodiscard]]
        std::string getVendor() const {
            if (_id < 0) {
                return "<unknown>";
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "manufacturer");
            std::string vendor;
            if (vendor_file.is_open()) {
                getline(vendor_file, vendor);
                return vendor;
            }
            return "<unknown>";
        }

        [[nodiscard]]
        std::string getModel() const {
            if (_id < 0) {
                return "<unknown>";
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "model_name");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                return value;
            }
            return "<unknown>";
        }

        [[nodiscard]]
        std::string getSerialNumber() const {
            if (_id < 0) {
                return "<unknown>";
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "serial_number");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                return value;
            }
            return "<unknown>";
        }

        [[nodiscard]]
        std::string getTechnology() const {
            if (_id < 0) {
                return "<unknown>";
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "technology");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                return value;
            }
            return "<unknown>";
        }

        [[nodiscard]]
        uint32_t getEnergyFull() const {
            if (_id < 0) {
                return 0;
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "energy_full");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                try {
                    return std::stoi(value);
                } catch (const std::invalid_argument& e) {
                    return 0;
                }
            }
            return 0;
        }

        [[nodiscard]]
        uint32_t getEnergyNow() const {
            if (_id < 0) {
                return 0;
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "energy_now");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                try {
                    return std::stoi(value);
                } catch (const std::invalid_argument& e) {
                    return 0;
                }
            }
            return 0;
        }

        [[nodiscard]]
        bool getCharging() const {
            if (_id < 0) {
                return false;
            }
            std::ifstream vendor_file(base_path + "BAT" + std::to_string(_id) + "/" + "status");
            std::string value;
            if (vendor_file.is_open()) {
                getline(vendor_file, value);
                return value == "Charging";
            }
            return false;
        }

        [[nodiscard]]
        bool getDischarging() const {
            return !charging();
        }


    };

    const std::string BatteryLinux::base_path = "/sys/class/power_supply/";

    using Battery = BatteryLinux;

}  // namespace hwinfo
