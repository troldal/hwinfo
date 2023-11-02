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

/**
 * @file RAMBase.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the RAMBase class template, which serves as the base class for RAM information retrieval.
 */
namespace hwinfo::detail
{
    template< typename IMPL >
    class RAMBase
    {
        friend IMPL;    ///< Allow the implementation class to access private members.

        /**
         * @struct RamItem
         * @brief Represents a single RAM module's information.
         */
        struct RamItem
        {
            std::string vendor {};          ///< The vendor or manufacturer of the RAM.
            std::string name {};            ///< The name or identifier of the RAM.
            std::string model {};           ///< The model number of the RAM.
            std::string serialNumber {};    ///< The serial number of the RAM.
            uint64_t    totalMem { 0 };     ///< The total memory capacity of the RAM in bytes.
        };

    public:
        /**
         * @brief Retrieves a constant reference to the vector of RamItem objects.
         * @return A constant reference to the vector of RamItem objects representing the RAM modules.
         */
        [[nodiscard]]
        std::vector< RamItem > const& items() const
        {
            return _items;
        }

        /**
         * @brief Retrieves the count of RAM modules.
         * @return The number of RAM modules.
         */
        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        /**
         * @brief Calculates the total memory capacity of all RAM modules.
         * @return The total memory capacity in bytes.
         */
        [[nodiscard]]
        int64_t totalMem() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint64_t(0),
                                   [](int64_t sum, const RamItem& item) { return sum + item.totalMem; });
        }

        /**
         * @brief Retrieves the amount of free memory.
         * @return The amount of free memory in bytes.
         */
        [[nodiscard]]
        int64_t freeMem() const
        {
            return _freeRam;
        }

        [[nodiscard]]
        std::string report() const
        {
            std::stringstream reportStream;

            reportStream << "Number of RAM modules: " << count() << "\n\n";

            size_t index = 1;
            for (const auto& item : items()) {
                reportStream << "RAM Module " << index++ << ":\n";
                reportStream << "\tVendor: " << item.vendor << "\n";
                reportStream << "\tName: " << item.name << "\n";
                reportStream << "\tModel: " << item.model << "\n";
                reportStream << "\tSerial Number: " << item.serialNumber << "\n";
                reportStream << "\tTotal Memory: " << item.totalMem << " bytes\n\n";
            }

            reportStream << "Total Memory: " << totalMem() << " bytes\n";
            reportStream << "Free Memory: " << freeMem() << " bytes\n";

            return reportStream.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const RAMBase& ram) { return os << ram.report(); }

        /**
         * @brief Retrieves RAM information.
         * @return An instance of IMPL containing the RAM information.
         */
        static IMPL getRamInfo() { return IMPL::getAllRam(); }

    protected:
        ~RAMBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        RAMBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Adds a RAM module's information to the list.
         * @param item The RAM module's information.
         */
        void addItem(const RamItem& item) { _items.push_back(item); }

        /**
         * @brief Provides non-const access to the derived class instance.
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the derived class instance.
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        uint64_t               _freeRam = -1;    ///< The amount of free memory in bytes.
        std::vector< RamItem > _items {};        ///< The list of RAM modules.
    };

}    // namespace hwinfo::detail
