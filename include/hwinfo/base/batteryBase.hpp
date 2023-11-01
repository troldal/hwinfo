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
 * @file batteryBase.hpp
 * @brief This file defines the CRTP base class for retrieving battery information on a PC.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @namespace hwinfo::detail
 * @brief The namespace for hardware information gathering classes and functions.
 */
namespace hwinfo::detail
{
    /**
     * @class BatteryBase
     * @brief CRTP base class for retrieving battery information on a PC.
     * @tparam IMPL The type of the derived class.
     *
     * This class provides the common interface and data storage for battery information.
     * The derived class should provide the implementation for retrieving the battery information.
     */
    template< typename IMPL >
    class BatteryBase
    {
        friend IMPL;    // Allow derived class to access private members.

        /**
         * @struct BatteryItem
         * @brief Represents a single battery's information.
         */
        struct BatteryItem
        {
            std::string vendor {};          ///< Vendor of the battery.
            std::string model {};           ///< Model of the battery.
            std::string serialNumber {};    ///< Serial number of the battery.
            std::string technology {};      ///< Battery technology type.
            std::string health {};          ///< Health status of the battery.
            std::string status {};          ///< Current status of the battery.
            uint32_t    capacity { 0 };     ///< Capacity of the battery in milliwatt-hours (mWh).
        };

    public:
        /**
         * @brief Retrieves the list of batteries and their information.
         * @return Constant reference to a vector of BatteryItem structs.
         */
        [[nodiscard]]
        std::vector< BatteryItem > const& items() const
        {
            return _items;
        }

        /**
         * @brief Retrieves the count of batteries.
         * @return The number of batteries.
         */
        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        /**
         * @brief Factory function to create and retrieve battery information.
         * @return An instance of the derived class with battery information filled.
         */
        [[nodiscard]]
        static IMPL getBatteryInfo()
        {
            return IMPL::getAllBatteries();
        }

    protected:
        ~BatteryBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        BatteryBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Adds a new battery item to the list.
         * @param item The battery item to be added.
         */
        void addItem(const BatteryItem& item) { _items.push_back(item); }

        /**
         * @brief Provides non-const access to the derived class instance.
         * @return Reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the derived class instance.
         * @return Constant reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        std::vector< BatteryItem > _items {};    ///< Storage for the list of battery information.
    };
}    // namespace hwinfo::detail
