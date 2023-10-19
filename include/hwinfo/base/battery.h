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

namespace hwinfo
{
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

    public:
        /**
         * @brief Constructor.
         *
         * @param id Identifier for the battery.
         */
        explicit BatteryBase(int8_t id = 0) : _id(id) {}

        /**
         * @brief Retrieves the current battery capacity.
         *
         * @return The current capacity as a ratio of energyNow to energyFull.
         */
        double capacity() { return static_cast< double >(energyNow()) / energyFull(); }

        /**
         * @brief Gets the battery vendor.
         *
         * @return Vendor name as a string.
         */
        [[nodiscard]]
        std::string vendor() const { return impl().getVendor();}

        /**
         * @brief Gets the battery model.
         *
         * @return Model name as a string.
         */
        [[nodiscard]]
        std::string model() const { return impl().getModel();}

        /**
         * @brief Gets the battery serial number.
         *
         * @return Serial number as a string.
         */
        [[nodiscard]]
        std::string serialNumber() const { return impl().getSerialNumber();}

        /**
         * @brief Gets the battery technology.
         *
         * @return Technology as a string.
         */
        [[nodiscard]]
        std::string technology() const { return impl().getTechnology();}

        /**
         * @brief Gets the total energy (in some unit) the battery can store when full.
         *
         * @return Energy value.
         */
        [[nodiscard]]
        uint32_t energyFull() const { return impl().getEnergyFull();}

        /**
         * @brief Gets the current energy (in some unit) the battery has.
         *
         * @return Energy value.
         */
        [[nodiscard]]
        uint32_t energyNow() const { return impl().getEnergyNow();}

        /**
         * @brief Checks if the battery is charging.
         *
         * @return True if charging, otherwise false.
         */
        [[nodiscard]]
        bool charging() const { return impl().getCharging();}

        /**
         * @brief Checks if the battery is discharging.
         *
         * @return True if discharging, otherwise false.
         */
        [[nodiscard]]
        bool discharging() const { return impl().getDischarging();}

        /**
         * @brief Retrieves all batteries.
         *
         * @return Vector containing all available batteries.
         */
        static std::vector< IMPL > getAllBatteries() { return IMPL::getAllBatteries_impl();}

    protected:
        /**
         * @brief Destructor.
         */
        ~BatteryBase() = default;

    private:
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

        int8_t      _id {};               ///< Battery identifier.
        std::string _vendor {};           ///< Battery vendor.
        std::string _model {};            ///< Battery model.
        std::string _serialNumber {};     ///< Battery serial number.
        std::string _technology {};       ///< Battery technology type.
        uint32_t    _energyFull { 0 };    ///< Total energy the battery can store when full.
    };
}    // namespace hwinfo

