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

/**
 * @file RAMWin.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the RAMWin class, which is a Windows-specific implementation for retrieving RAM information.
 */
namespace hwinfo
{
    namespace detail
    {

        /**
         * @class RAMWin
         * @brief Windows-specific implementation for retrieving RAM (Random Access Memory) information.
         *
         * This class extends the RAMBase class template, providing functionalities
         * specific to Windows for retrieving information about RAM modules.
         */
        class RAMWin : public RAMBase< RAMWin >
        {
            using BASE = RAMBase< RAMWin >;    ///< Type alias for the base class.
            friend BASE;                       ///< Allow the base class to access private members.

            /**
             * @brief Default constructor is private to ensure controlled object creation.
             */
            RAMWin() = default;

            /**
             * @brief Retrieves information about all RAM modules in the system.
             * @return A RAMWin instance populated with information about all RAM modules.
             */
            [[nodiscard]]
            static RAMWin getAllRam()
            {
                RAMWin ram_blocks;

                using namespace WMI;

                // Querying RAM information using Windows Management Instrumentation (WMI)
                auto info =
                    wmiInterface
                        .query< RamInfo::MANUFACTURER, RamInfo::NAME, RamInfo::PARTNUMBER, RamInfo::SERIALNUMBER, RamInfo::CAPACITY >();

                // Iterating through the query results and populating the RAMWin instance
                for (const auto& block : info) {
                    auto item = BASE::RamItem {};

                    item.vendor       = std::get< 0 >(block);
                    item.name         = std::get< 1 >(block);
                    item.model        = std::get< 2 >(block);
                    item.serialNumber = std::get< 3 >(block);
                    item.totalMem     = std::stoll(std::get< 4 >(block));

                    ram_blocks.addItem(item);
                }

                // Querying the total free memory in the system
                auto result         = wmiInterface.query< OSInfo::FREEMEMORY >();
                ram_blocks._freeRam = (result.empty()) ? 0 : std::stoll(std::get< 0 >(result.front())) * 1024;

                return ram_blocks;
            }

            inline static WMI::WMIInterface wmiInterface {};    ///< Static instance of the WMI interface for querying system information.
        };

        //        WMI::WMIInterface RAMWin::wmiInterface {};    ///< Definition of the static WMI interface instance.

    }    // namespace detail

    using RAM = detail::RAMWin;    ///< Typedef for easier access to the RAMWin class outside of the detail namespace.

}    // namespace hwinfo
