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

    // `BatteryBase` Class
    // 1. Encapsulation:
    //    - The derived class (`BatteryWin`) can directly modify the base class attributes (`_vendor`, `_model`, `_serialNumber`,
    //    `_technology`, `_health`). This is a design choice in CRTP, but you might still want to ensure that data integrity is maintained.
    //
    // 2. Error Handling:
    //    - The methods in the base class do not handle or convey any errors. Depending on the usage, you might consider exceptions, error
    //    codes, or `std::optional`/`std::variant` for error reporting.
    //
    // 3. Lack of Initialization:
    //    - The attributes of the `BatteryBase` class are not initialized in the constructor, which might lead to undefined behavior if
    //    accessed before being set.
    //
    // 4. Incomplete API:
    //    - Depending on the intended functionality, the class might be missing some battery-related features (e.g., setting thresholds,
    //    retrieving temperature, charge/discharge rates, etc.).

    /**
     * @class BatteryBase
     * @brief Base class for battery information retrieval using the Curiously Recurring Template Pattern (CRTP).
     *
     * @tparam IMPL The derived class type which should implement the battery retrieval logic.
     */
    template < typename IMPL >
    class BatteryBase
    {
        friend IMPL;

        struct BatteryElementInfo
        {
            std::string vendor {};
            std::string model {};
            std::string serialNumber {};
            std::string technology {};
            std::string health {};
            std::string status {};
            uint32_t    capacity { 0 };
        };

    public:
        std::vector< BatteryElementInfo > const& items() const { return _items; }

        void addItem(const BatteryElementInfo& item) { _items.push_back(item); }

        /**
         * @brief Retrieves all batteries.
         *
         * @return Vector containing all available batteries.
         */
        static IMPL getBatteryInfo() { return IMPL::getAllBatteries(); }

    protected:
        /**
         * @brief Destructor.
         */
        ~BatteryBase() = default;

    private:
        /**
         * @brief Constructor.
         *
         * @param id Identifier for the battery.
         */
        BatteryBase() = default;

        /**
         * @brief Provides access to the implementation-specific methods in the derived class.
         *
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast<IMPL&>( *this ); }

        /**
         * @brief Provides const access to the implementation-specific methods in the derived class.
         *
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast<IMPL const&>( *this ); }

        std::vector< BatteryElementInfo > _items {};
    };
}    // namespace hwinfo

