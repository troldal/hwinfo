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

#include <string>

namespace hwinfo::detail
{
    template< typename IMPL >
    class OSBase
    {
        friend IMPL;

        struct OSItem
        {
            std::string fullName {};
            std::string name {};
            std::string version {};
            std::string kernel {};
            bool        _32bit        = false;
            bool        _64bit        = false;
            bool        _bigEndian    = false;
            bool        _littleEndian = false;
        };

    public:
        std::string fullName() { return IMPL::getFullName(); }

        std::string name() { return IMPL::getName(); }

        std::string version() { return IMPL::getVersion(); }

        std::string kernel() { return IMPL::getKernel(); }

        [[nodiscard]]
        bool is32bit() const
        {
            return _item._32bit;
        }
        [[nodiscard]]
        bool is64bit() const
        {
            return _item._64bit;
        }
        [[nodiscard]]
        bool isBigEndian() const
        {
            return _item._bigEndian;
        }
        [[nodiscard]]
        bool isLittleEndian() const
        {
            return _item._littleEndian;
        }

        static IMPL getOSInfo() { return IMPL::getAllOSs(); }

    protected:
        ~OSBase() = default;

    private:
        OSBase() = default;

        /**
         * @brief Provides access to the implementation-specific methods in the derived class.
         *
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the implementation-specific methods in the derived class.
         *
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        OSItem _item {};
    };

}    // namespace hwinfo::detail
