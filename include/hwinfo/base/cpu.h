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
#include <string>
#include <vector>

namespace hwinfo::detail
{
    template< typename IMPL >
    class CPUBase
    {
        friend IMPL;

    public:
        [[nodiscard]]
        int id() const
        {
            return impl().getId();
        }

        [[nodiscard]]
        const std::string& modelName() const
        {
            return impl().getModelName();
        }

        [[nodiscard]]
        const std::string& vendor() const
        {
            return impl().getVendor();
        }

        [[nodiscard]]
        int64_t L1CacheSize_Bytes() const
        {
            return impl().getL1CacheSize();
        }

        [[nodiscard]]
        int64_t L2CacheSize_Bytes() const
        {
            return impl().getL2CacheSize();
        }

        [[nodiscard]]
        int64_t L3CacheSize_Bytes() const
        {
            return impl().getL3CacheSize();
        }

        [[nodiscard]]
        int numPhysicalCores() const
        {
            return impl().getNumPhysicalCores();
        }

        [[nodiscard]]
        int numLogicalCores() const
        {
            return impl().getNumLogicalCores();
        }

        [[nodiscard]]
        int64_t maxClockSpeed_MHz() const
        {
            return impl().getMaxClockSpeed();
        }

        [[nodiscard]]
        int64_t regularClockSpeed_MHz() const
        {
            return impl().getRegularClockSpeed();
        }

        [[nodiscard]]
        int64_t currentClockSpeed_MHz(int thread_id) const
        {
            return impl().getCurrentClockSpeed(thread_id);
        }

        [[nodiscard]]
        std::vector< int64_t > currentClockSpeed_MHz() const
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
        const std::vector< std::string >& flags() const
        {
            return impl().getFlags();
        }

        static std::vector< IMPL > getAllCPUs() { return IMPL::getAllCPUs_impl(); }

    protected:
        ~CPUBase() = default;

    private:
        CPUBase() = default;

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

        int                        _id { -1 };
        std::string                _modelName;
        std::string                _vendor;
        int                        _numPhysicalCores { -1 };
        int                        _numLogicalCores { -1 };
        int64_t                    _maxClockSpeed_MHz { -1 };
        int64_t                    _regularClockSpeed_MHz { -1 };
        int64_t                    _L1CacheSize_Bytes { -1 };
        int64_t                    _L2CacheSize_Bytes { -1 };
        int64_t                    _L3CacheSize_Bytes { -1 };
        std::vector< std::string > _flags {};
    };
}    // namespace hwinfo
