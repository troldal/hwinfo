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
 * @file DiskWin.hpp
 * @namespace hwinfo
 * @brief Contains the implementation of the DiskWin class, a Windows-specific implementation for disk information retrieval.
 */

#pragma once

#include "../base/diskBase.hpp"
#include "utils/WMIInterface.hpp"

namespace hwinfo
{

    namespace detail
    {

        /**
         * @class DiskWin
         * @brief A Windows-specific implementation of the DiskBase class for disk information retrieval.
         *
         * This class is derived from DiskBase using the Curiously Recurring Template Pattern (CRTP) and provides
         * an implementation specific to the Windows operating system for retrieving disk information using WMI.
         */
        class DiskWin : public DiskBase< DiskWin >
        {
            using BASE = DiskBase< DiskWin >;    ///< Typedef for the base class.
            friend BASE;                         ///< Grants access to the base class for certain operations.

            /**
             * @brief Default constructor. It is private to ensure only methods within the class can create an instance.
             */
            DiskWin() = default;

        private:
            /**
             * @brief Retrieves information about all disks on the system.
             * @return DiskWin An instance of DiskWin containing the information about all disks.
             *
             * This function queries the system for information about all disks using WMI,
             * and then populates a DiskWin instance with this information.
             */
            static DiskWin getAllDisks()
            {
                DiskWin disks;    ///< Instance of DiskWin to hold the disk information.

                using namespace WMI;

                // Query disk information using WMI.
                auto info = wmiInterface.query< DiskInfo::MODEL, DiskInfo::MANUFACTURER, DiskInfo::SERIALNUMBER, DiskInfo::SIZE >();

                for (const auto& disk : info) {
                    auto item = BASE::DiskItem {};    ///< Temporary DiskItem to hold information about a single disk.

                    item.model        = std::get< 0 >(disk);                 ///< Set the model of the disk.
                    item.vendor       = std::get< 1 >(disk);                 ///< Set the vendor of the disk.
                    item.serialNumber = std::get< 2 >(disk);                 ///< Set the serial number of the disk.
                    item.size         = std::stoull(std::get< 3 >(disk));    ///< Set the size of the disk.

                    disks.addItem(item);    ///< Add the DiskItem to the collection of disks.
                }

                return disks;    ///< Return the instance of DiskWin containing the disk information.
            }

            static WMI::WMIInterface wmiInterface;    ///< Static instance of WMIInterface for querying WMI.
        };

        WMI::WMIInterface DiskWin::wmiInterface {};    ///< Initialization of the static WMIInterface member.

    }    // namespace detail

    using Disk = detail::DiskWin;    ///< Typedef for easier access to the DiskWin class outside of the detail namespace.

}    // namespace hwinfo
