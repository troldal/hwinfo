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

/**
 * @file diskBase.hpp
 * @namespace hwinfo::detail
 * @brief Contains the implementation of the DiskBase class.
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace hwinfo::detail
{
    /**
     * @class DiskBase
     * @brief CRTP base class template for retrieving disk information.
     *
     * This class serves as a base for deriving platform-specific implementations
     * to gather disk information. It uses the Curiously Recurring Template Pattern (CRTP)
     * to enable static polymorphism.
     *
     * @tparam IMPL The type of the derived class.
     */
    template< typename IMPL >
    class DiskBase
    {
        friend IMPL;    ///< Allows the derived class to access the private members of this class.

        /**
         * @struct DiskItem
         * @brief Represents information about a single disk.
         */
        struct DiskItem
        {
            std::string vendor {};          ///< The disk's vendor.
            std::string model {};           ///< The disk's model.
            std::string serialNumber {};    ///< The disk's serial number.
            uint64_t    size { 0 };         ///< The disk's size in bytes.
        };

    public:
        /**
         * @brief Retrieves a constant reference to the vector of DiskItem structs.
         * @return const std::vector<DiskItem>& The vector of DiskItem structs.
         */
        [[nodiscard]]
        std::vector< DiskItem > const& items() const
        {
            return _items;
        }

        /**
         * @brief Retrieves the number of disks in the collection.
         * @return size_t The number of disks.
         */
        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        /**
         * @brief Calculates the total disk space of all disks in the collection.
         * @return int64_t The total disk space in bytes.
         */
        [[nodiscard]]
        auto totalDiskSpace() const
        {
            return std::accumulate(_items.begin(), _items.end(), int64_t(0),
                                   [](int64_t sum, const DiskItem& item) { return sum + item.size; });
        }

        /**
         * @brief Generates a report of the disk information.
         * @return std::string A string containing the disk information.
         */
        [[nodiscard]]
        std::string report() const
        {
            std::stringstream reportStream;

            reportStream << "Number of disks: " << count() << std::endl;
            reportStream << "Total disk space: " << totalDiskSpace() / std::pow(1024, 3) << " GB\n\n";

            size_t index = 1;
            for (const auto& disk : items()) {
                reportStream << "Disk " << index++ << " Information:\n";
                reportStream << "\tVendor: " << disk.vendor << "\n";
                reportStream << "\tModel: " << disk.model << "\n";
                reportStream << "\tSerial Number: " << disk.serialNumber << "\n";
                reportStream << "\tSize: " << disk.size / std::pow(1024, 3) << " GB\n\n";
            }
            return reportStream.str();
        }

        /**
         * @brief Overloaded stream operator to print the disk information.
         * @param os The output stream to write to.
         * @param diskBase The DiskBase object to print.
         * @return std::ostream& The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const DiskBase& diskBase) { return os << diskBase.report(); }

        /**
         * @brief Static function to retrieve disk information.
         * @return IMPL An instance of the derived class with disk information populated.
         */
        [[nodiscard]]
        static IMPL getDiskInfo()
        {
            return IMPL::getAllDisks();
        }

    protected:
        ~DiskBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        DiskBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Adds a DiskItem to the collection.
         * @param item The DiskItem to add.
         */
        void addItem(const DiskItem& item) { _items.push_back(item); }

        /**
         * @brief Provides non-const access to the derived class instance.
         * @return IMPL& Reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the derived class instance.
         * @return const IMPL& Constant reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        std::vector< DiskItem > _items {};    ///< Collection of DiskItems representing each disk.
    };

}    // namespace hwinfo::detail
