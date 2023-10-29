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
            return m_vendor;
        }

        [[nodiscard]]
        std::string name() const
        {
            return m_name;
        }

        [[nodiscard]]
        std::string driverVersion() const
        {
            return m_driverVersion;
        }

        [[nodiscard]]
        int64_t memory_Bytes() const
        {
            return m_memory;
        }

        [[nodiscard]]
        int64_t frequency_MHz() const
        {
            return m_frequency;
        }

        [[nodiscard]]
        int num_cores() const
        {
            return m_num_cores;
        }

        [[nodiscard]]
        int id() const
        {
            return m_id;
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
        std::string m_vendor {};
        std::string m_name {};
        std::string m_driverVersion {};
        int64_t     m_memory { 0 };
        int64_t     m_frequency { 0 };
        int         m_num_cores { 0 };
        int         m_id { 0 };

        std::string m_vendor_id {};
        std::string m_device_id {};
    };


}    // namespace hwinfo::detail
