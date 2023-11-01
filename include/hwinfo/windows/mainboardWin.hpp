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

#include "../base/mainboardBase.hpp"
#include "utils/WMIInterface.hpp"

/**
 * @file MainBoardWin.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the MainBoardWin class, the Windows-specific implementation for mainboard information retrieval.
 */
namespace hwinfo
{
    namespace detail
    {

        /**
         * @class MainBoardWin
         * @brief Windows-specific implementation of the MainBoardBase class for mainboard (motherboard) information retrieval.
         *
         * This class provides the Windows-specific implementation for retrieving mainboard information using Windows Management
         * Instrumentation (WMI).
         */
        class MainBoardWin : public MainBoardBase< MainBoardWin >
        {
            using BASE = MainBoardBase< MainBoardWin >;    ///< Alias for the base class type.
            friend BASE;                                   ///< Make the base class a friend to allow it to access private members.

            /**
             * @brief Default constructor is private to prevent instantiation.
             */
            MainBoardWin() = default;

        private:
            /**
             * @brief Retrieves information about all baseboards (mainboards) in the system.
             *
             * Uses Windows Management Instrumentation (WMI) to query the system for mainboard information.
             *
             * @return An instance of MainBoardWin populated with mainboard information.
             */
            [[nodiscard]]
            static MainBoardWin getAllBaseboards()
            {
                MainBoardWin boards;

                using namespace WMI;
                auto info =
                    wmiInterface.query< BoardInfo::MANUFACTURER, BoardInfo::PRODUCT, BoardInfo::VERSION, BoardInfo::SERIALNUMBER >();

                auto board = info.front();
                auto item  = BASE::MainBoardItem {};

                item.vendor       = std::get< 0 >(board);
                item.name         = std::get< 1 >(board);
                item.version      = std::get< 2 >(board);
                item.serialNumber = std::get< 3 >(board);

                boards._item = item;

                return boards;
            }

            static WMI::WMIInterface wmiInterface;    ///< Static instance of the WMI interface for querying system information.
        };

        WMI::WMIInterface MainBoardWin::wmiInterface {};    ///< Definition of the static WMI interface instance.

    }    // namespace detail

    using MainBoard = detail::MainBoardWin;    ///< Typedef for easier access to the GPUWin class outside of the detail namespace.

}    // namespace hwinfo
