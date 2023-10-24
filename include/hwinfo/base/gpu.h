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
#include <vector>

namespace hwinfo::detail
{

    template< typename IMPL >
    class GPUBase
    {

        friend IMPL;

    public:

        [[nodiscard]]
        std::string vendor() const
        {
            return impl().getVendor();
        }

        [[nodiscard]]
        std::string name() const
        {
            return impl().getName();
        }

        [[nodiscard]]
        std::string driverVersion() const
        {
            return impl().getDriverVersion();
        }

        [[nodiscard]]
        int64_t memory_Bytes() const
        {
            return impl().getMemory();
        }

        [[nodiscard]]
        int64_t frequency_MHz() const
        {
            return impl().getFrequency();
        }

        [[nodiscard]]
        int num_cores() const
        {
            return impl().getNumCores();
        }

        [[nodiscard]]
        int id() const
        {
            return impl().getId();
        }

        static std::vector< IMPL > getAllGPUs() { return IMPL::getAllGPUs_impl(); }

    protected:
        ~GPUBase() = default;

    private:
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

        GPUBase() = default;
        std::string _vendor {};
        std::string _name {};
        std::string _driverVersion {};
        int64_t     _memory_Bytes { 0 };
        int64_t     _frequency_MHz { 0 };
        int         _num_cores { 0 };
        int         _id { 0 };

        std::string _vendor_id {};
        std::string _device_id {};
    };


}    // namespace hwinfo::detail
