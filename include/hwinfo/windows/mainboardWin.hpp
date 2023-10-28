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

        public:
            MainBoardWin()
            {
                BASE::_vendor       = getVendor();
                BASE::_name         = getName();
                BASE::_version      = getVersion();
                BASE::_serialNumber = getSerialNumber();
            }

        private:
            [[nodiscard]]
            static std::string getVendor()
            {
                auto result = s_wmi.query< std::string >("Win32_BaseBoard", "Manufacturer");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            [[nodiscard]]
            static std::string getName()
            {
                auto result = s_wmi.query< std::string >("Win32_BaseBoard", "Product");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            [[nodiscard]]
            static std::string getVersion()
            {
                auto result = s_wmi.query< std::string >("Win32_BaseBoard", "Version");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            [[nodiscard]]
            static std::string getSerialNumber()
            {
                auto result = s_wmi.query< std::string >("Win32_BaseBoard", "SerialNumber");
                if (result.empty()) return "<unknown>";

                return result.front();
            }

            static WMI::WMIInterface s_wmi;
        };

        WMI::WMIInterface MainBoardWin::s_wmi {};

    }    // namespace detail

    using MainBoard = detail::MainBoardWin;

}    // namespace hwinfo
