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

/**
 * @file batteryWin.hpp
 * @brief This file defines the class for retrieving battery information on Windows systems.
 */

#pragma once

#include "../base/batteryBase.hpp"
#include "utils/WMIInterface.hpp"

/**
 * @namespace hwinfo
 * @brief The namespace for hardware information gathering.
 */
namespace hwinfo
{

    /**
     * @namespace detail
     * @brief Namespace for implementation details.
     */
    namespace detail
    {
        /**
         * @class BatteryWin
         * @brief Derived class for retrieving battery information on Windows systems.
         *
         * This class uses Windows Management Instrumentation (WMI) to retrieve battery information.
         */
        class BatteryWin : public BatteryBase< BatteryWin >
        {
            using BASE = BatteryBase< BatteryWin >;    ///< Type alias for the base class.
            friend BASE;                               ///< Make base class a friend to allow access to private constructor.

            /**
             * @brief Private constructor to control object creation.
             */
            explicit BatteryWin() = default;

            /**
             * @brief Retrieves all battery information on the system using WMI.
             * @return BatteryWin instance filled with battery information.
             */
            static BatteryWin getAllBatteries()
            {
                BatteryWin batteries;    // Create an instance to store battery information.

                using namespace WMI;

                // Query battery information using WMI.
                auto info = wmiInterface.query< BatteryInfo::NAME, BatteryInfo::CHEMISTRY, BatteryInfo::HEALTH, BatteryInfo::STATUS,
                                                BatteryInfo::CAPACITY >();

                // Iterate over the retrieved information and populate the BatteryWin instance.
                for (const auto& battery : info) {
                    auto item = BASE::BatteryItem {};    // Create a BatteryItem struct to store information.

                    // Assign retrieved values to the corresponding members of BatteryItem.
                    item.model      = std::get< 0 >(battery);
                    item.technology = to_string(std::get< 1 >(battery));
                    item.health     = std::get< 2 >(battery);
                    item.status     = to_string(std::get< 3 >(battery));
                    item.capacity   = std::get< 4 >(battery);

                    batteries.addItem(item);    // Add the filled BatteryItem to the BatteryWin instance.
                }

                return batteries;    // Return the filled BatteryWin instance.
            }

            static WMI::WMIInterface wmiInterface;    ///< Static instance of WMIInterface for querying WMI.
        };

        // Initialize the static WMIInterface instance.
        WMI::WMIInterface BatteryWin::wmiInterface {};
    }    // namespace detail

    // Alias the BatteryWin class in the hwinfo namespace for easier access.
    using Battery = detail::BatteryWin;

}    // namespace hwinfo
