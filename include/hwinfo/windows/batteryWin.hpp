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

        // `BatteryWin` Class
        // 1. Dependency on WMI:
        //    - The class is tightly coupled with the Windows Management Instrumentation (WMI) interface. This is typical for
        //    platform-specific implementations but consider this in case portability becomes a requirement.
        //
        // 2. Lack of Error Handling:
        //    - The `getCapacity` and `getStatus` methods do not handle errors explicitly. They return an empty string or zero, which might
        //    be ambiguous.
        //
        // 3. Magic Values:
        //    - The methods return default values (`{}` or empty string) in case of an error or if no value is found. Define these values
        //    explicitly as constants or use `std::optional`.
        //
        // 4. Data Synchronization:
        //    - The `getAllBatteries_impl` method populates the battery objects but does not ensure that the data in each vector (`models`,
        //    `chemistry`, `health`) is synchronized or has the same size.
        //
        // 5. Direct Data Modification:
        //    - The `getAllBatteries_impl` method directly sets the private members of `BatteryWin` instances. This is acceptable in CRTP,
        //    but ensure consistency and data integrity.
        //
        // 6. Lack of Comprehensive Documentation:
        //    - Ensure that all methods, especially those interacting with external libraries or APIs, are well-documented.
        //
        // 7. Resource Management:
        //    - Ensure that the `wmiInterface` is properly managing its resources, especially if it's acquiring system handles or other
        //    resources.

        // General Recommendations
        // - Unit Tests: Design the code in a way that allows for unit testing, possibly by abstracting dependencies and using mock objects.
        // - Consistent Coding Style: Maintain a consistent coding style throughout the codebase.
        // - Use of Modern C++ Features: Leverage modern C++ features where appropriate.
        // - Memory Management: Ensure all dynamically allocated memory is properly managed.

        /**
         * @class BatteryWin
         * @brief Specific implementation of BatteryBase for Windows systems.
         *
         * This class inherits from BatteryBase and provides the specific implementations required to retrieve battery information on
         * Windows systems.
         */
        class BatteryWin : public BatteryBase< BatteryWin >
        {
            using BASE = BatteryBase< BatteryWin >;

            friend BASE;

        public:
            /**
             * @brief Default constructor for BatteryWin.
             */
            explicit BatteryWin() = default;

        private:
            /**
             * @brief Retrieves the current capacity of the battery.
             *
             * This method queries the WMI interface to obtain the current capacity of the battery and returns it.
             *
             * @return The current battery capacity as a percentage.
             *
             * @note This method is marked as [[nodiscard]] to encourage checking the returned value.
             */
            [[nodiscard]]
            uint32_t getCapacity() const    // NOLINT
            {
                using namespace WMI;
                auto res = wmiInterface.queryValue< BatteryInfo::CAPACITY >();
                if (res.empty()) return {};

                return res.front();
            }

            /**
             * @brief Retrieves the status of the battery.
             *
             * This method queries the WMI interface to obtain the current status of the battery and returns it as a string.
             *
             * @return The current status of the battery.
             *
             * @note This method is marked as [[nodiscard]] to encourage checking the returned value.
             */
            [[nodiscard]]
            std::string getStatus() const    // NOLINT
            {
                using namespace WMI;
                auto res = wmiInterface.queryValue< BatteryInfo::STATUS >();
                if (res.empty()) return {};

                return to_string(res.front());
            }

            /**
             * @brief Retrieves information of all available batteries.
             *
             * This method queries the WMI interface to obtain a list of all available batteries and their information.
             *
             * @return A vector of BatteryWin objects, each representing a battery.
             */
            static std::vector< BatteryWin > getAllBatteries_impl()
            {
                std::vector< BatteryWin > batteries;

                using namespace WMI;
                auto info = wmiInterface.queryRecord< BatteryInfo::NAME, BatteryInfo::CHEMISTRY, BatteryInfo::HEALTH >();

                for (const auto& batt : info) {
                    batteries.emplace_back();
                    auto& battery = batteries.back();

                    battery._model      = std::get< 0 >(batt);
                    battery._technology = to_string(std::get< 1 >(batt));
                    battery._health     = std::get< 2 >(batt);
                }

                return batteries;
            }

            /**
             * @brief Interface to the Windows Management Instrumentation (WMI).
             */
            static WMI::WMIInterface wmiInterface;
        };

        /**
         * @brief Static instance of WMIInterface used to interact with the Windows Management Instrumentation (WMI).
         */
        WMI::WMIInterface BatteryWin::wmiInterface {};
    }    // namespace detail

    /**
     * @typedef Battery
     * @brief Alias for the BatteryWin class, providing a convenient way to refer to it.
     */
    using Battery = detail::BatteryWin;

}    // namespace hwinfo
