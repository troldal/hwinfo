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
#include <vector>

namespace hwinfo::detail
{

    template< typename IMPL >
    class GPUBase
    {
        friend IMPL;

        struct GpuItem
        {
            std::string vendor {};
            std::string name {};
            std::string driverVersion {};
            int64_t     memory { 0 };
            int64_t     frequency { 0 };
            int         num_cores { 0 };
            int         id { 0 };
        };

    public:
        [[nodiscard]]
        std::vector< GpuItem > const& items() const { return _items; }

        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        [[nodiscard]]
        auto coreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0), [](uint32_t sum, const GpuItem& item) {
                return sum + item.num_cores;
            });
        }

        static IMPL getGpuInfo() { return IMPL::getAllGPUs(); }

    protected:
        ~GPUBase() = default;

    private:
        GPUBase() = default;

        void addItem(const GpuItem& item) { _items.push_back(item); }

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

        std::vector< GpuItem > _items {};
    };

}    // namespace hwinfo::detail
