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

#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace hwinfo::detail
{
    template< typename IMPL >
    class CPUBase
    {
        friend IMPL;

        struct CpuItem
        {
            std::string                vendor {};
            std::string                modelName {};
            int64_t                    L1CacheSize { -1 };
            int64_t                    L2CacheSize { -1 };
            int64_t                    L3CacheSize { -1 };
            uint32_t                   numPhysicalCores { 0 };
            uint32_t                   numLogicalCores { 0 };
            int64_t                    maxClockSpeed { -1 };
            int64_t                    regularClockSpeed { -1 };
            std::vector< std::string > flags {};
        };

    public:
        [[nodiscard]]
        std::vector< CpuItem > const& items() const { return _items; }

        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        [[nodiscard]]
        auto physicalCoreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0), [](uint32_t sum, const CpuItem& item) {
                return sum + item.numPhysicalCores;
            });
        }

        [[nodiscard]]
        auto logicalCoreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0), [](uint32_t sum, const CpuItem& item) {
                return sum + item.numLogicalCores;
            });
        }

        [[nodiscard]]
        int64_t currentClockSpeed(int thread_id) const
        {
            return impl().getCurrentClockSpeed(thread_id);
        }

        [[nodiscard]]
        std::vector< int64_t > currentClockSpeed() const
        {
            return impl().getCurrentClockSpeed();
        }

        [[nodiscard]]
        double currentUtilisation() const
        {
            return impl().getCurrentUtilisation();
        }

        [[nodiscard]]
        double threadUtilisation(int thread_index) const
        {
            return impl().getThreadUtilisation(thread_index);
        }

        [[nodiscard]]
        std::vector< double > threadsUtilisation() const
        {
            return impl().getThreadsUtilisation();
        }

        [[nodiscard]]
        static IMPL getCpuInfo()
        {
            return IMPL::getAllCPUs();
        }

    protected:
        ~CPUBase() = default;

    private:
        CPUBase() = default;

        void addItem(const CpuItem& item) { _items.push_back(item); }

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

        std::vector< CpuItem > _items {};
    };
}    // namespace hwinfo::detail
