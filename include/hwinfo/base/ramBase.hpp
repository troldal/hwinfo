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
#include <numeric>
#include <string>

namespace hwinfo::detail
{
    template< typename IMPL >
    class RAMBase
    {
        friend IMPL;

        struct RamBlockInfo
        {
            std::string vendor {};
            std::string name {};
            std::string model {};
            std::string serialNumber {};
            uint64_t    totalMem { 0 };
        };

    public:
        std::vector< RamBlockInfo > const& items() const { return _items; }

        [[nodiscard]]
        int64_t totalMem() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint64_t(0), [](int64_t sum, const RamBlockInfo& item) {
                return sum + item.totalMem;
            });
        }

        [[nodiscard]]
        int64_t freeMem() const
        {
            return _freeRam;
        }

        static IMPL getRamInfo() { return IMPL::getAllRam(); }

    protected:
        ~RAMBase() = default;

    private:
        RAMBase() = default;

        void addItem(const RamBlockInfo& item) { _items.push_back(item); }

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

        uint64_t                    _freeRam = -1;
        std::vector< RamBlockInfo > _items {};
    };

}    // namespace hwinfo::detail
