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

namespace hwinfo
{
    namespace detail
    {

        class MainBoardWin : public MainBoardBase< MainBoardWin >
        {
            using BASE = MainBoardBase< MainBoardWin >;
            friend BASE;

            MainBoardWin() = default;

        private:
            [[nodiscard]]
            static MainBoardWin getAllBaseboards()
            {
                MainBoardWin boards;

                using namespace WMI;
                auto info =
                    wmiInterface.query< BoardInfo::MANUFACTURER, BoardInfo::PRODUCT, BoardInfo::VERSION, BoardInfo::SERIALNUMBER >();

                auto board   = info.front();
                auto current = BASE::MainBoardItem {};

                current.vendor       = std::get< 0 >(board);
                current.name         = std::get< 1 >(board);
                current.version      = std::get< 2 >(board);
                current.serialNumber = std::get< 3 >(board);

                boards._item = current;

                return boards;
            }

            static WMI::WMIInterface wmiInterface;
        };

        WMI::WMIInterface MainBoardWin::wmiInterface {};

    }    // namespace detail

    using MainBoard = detail::MainBoardWin;

}    // namespace hwinfo
