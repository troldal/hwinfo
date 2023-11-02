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

#include <string>
#include <vector>

/**
 * @file MainBoardBase.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the MainBoardBase class template, a CRTP base class for mainboard (motherboard) information retrieval.
 */
namespace hwinfo::detail
{

    /**
     * @class MainBoardBase
     * @brief Curiously Recurring Template Pattern (CRTP) base class for retrieving mainboard (motherboard) information.
     *
     * This template class defines the common interface and data structures for retrieving mainboard information,
     * to be implemented by platform-specific derived classes.
     *
     * @tparam IMPL The type of the derived class implementing the mainboard information retrieval.
     */
    template< typename IMPL >
    class MainBoardBase
    {
        friend IMPL;    ///< Make the derived class a friend to allow it to access private members.

        /**
         * @struct MainBoardItem
         * @brief Structure holding information about the mainboard.
         */
        struct MainBoardItem
        {
            std::string vendor {};          ///< The vendor/manufacturer of the mainboard.
            std::string name {};            ///< The name of the mainboard.
            std::string version {};         ///< The version of the mainboard.
            std::string serialNumber {};    ///< The serial number of the mainboard.
        };

    public:
        /**
         * @brief Retrieves the vendor of the mainboard.
         * @return A string representing the mainboard's vendor.
         */
        [[nodiscard]]
        std::string vendor() const
        {
            return _item.vendor;
        }

        /**
         * @brief Retrieves the name of the mainboard.
         * @return A string representing the mainboard's name.
         */
        [[nodiscard]]
        std::string name() const
        {
            return _item.name;
        }

        /**
         * @brief Retrieves the version of the mainboard.
         * @return A string representing the mainboard's version.
         */
        [[nodiscard]]
        std::string version() const
        {
            return _item.version;
        }

        /**
         * @brief Retrieves the serial number of the mainboard.
         * @return A string representing the mainboard's serial number.
         */
        [[nodiscard]]
        std::string serialNumber() const
        {
            return _item.serialNumber;
        }

        /**
         * @brief Generates a report of the mainboard information.
         * @return A string containing the mainboard information.
         */
        [[nodiscard]]
        std::string report() const
        {
            std::stringstream reportStream;

            reportStream << "Mainboard Information:\n";
            reportStream << "\tVendor: " << vendor() << "\n";
            reportStream << "\tName: " << name() << "\n";
            reportStream << "\tVersion: " << version() << "\n";
            reportStream << "\tSerial Number: " << serialNumber() << "\n";

            return reportStream.str();
        }

        /**
         * @brief Overloaded stream operator to print the mainboard information.
         * @param os The output stream to write to.
         * @param mainboard The mainboard object to print.
         * @return A reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const MainBoardBase& mainboard) { return os << mainboard.report(); }

        /**
         * @brief Factory method to retrieve mainboard information.
         * @return An instance of the IMPL type, populated with mainboard information.
         */
        static IMPL getBaseboardInfo() { return IMPL::getAllBaseboards(); }

    protected:
        ~MainBoardBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        MainBoardBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Retrieves a non-const reference to the derived class instance.
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Retrieves a const reference to the derived class instance.
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        MainBoardItem _item {};    ///< The data structure holding the mainboard information.
    };

}    // namespace hwinfo::detail
