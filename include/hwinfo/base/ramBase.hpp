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

#include <cstdint>
#include <string>

namespace hwinfo::detail
{
    template< typename IMPL >
    class RAMBase
    {
        friend IMPL;

    public:
        [[nodiscard]]
        std::string vendor() const
        {
            return _vendor;
        }

        [[nodiscard]]
        std::string name() const
        {
            return _name;
        }

        [[nodiscard]]
        std::string model() const
        {
            return _model;
        }

        [[nodiscard]]
        std::string serialNumber() const
        {
            return _serialNumber;
        }

        [[nodiscard]]
        int64_t total_Bytes() const
        {
            return _total_Bytes;
        }

        [[nodiscard]]
        int64_t free_Bytes() const
        {
            return _free_Bytes;
        }

        [[nodiscard]]
        int64_t available_Bytes() const
        {
            return _available_Bytes;
        }

    protected:
        ~RAMBase() = default;

    private:
        RAMBase() = default;

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

        std::string _vendor {};
        std::string _name {};
        std::string _model {};
        std::string _serialNumber {};
        uint64_t    _total_Bytes     = -1;
        uint64_t    _free_Bytes      = -1;
        uint64_t    _available_Bytes = -1;
        int         _frequency       = -1;
    };

}    // namespace hwinfo::detail
