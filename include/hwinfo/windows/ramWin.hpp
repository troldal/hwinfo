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

#include "../base/ramBase.hpp"

// #include <windows.h>
//
// #include "hwinfo/windows/utils/WMIwrapper.hpp"
//
// #include <string>
// #include <vector>
//
// #include "../utils/stringutils.hpp"
// #include "utils/wmi_wrapper.hpp"

namespace hwinfo
{
    namespace detail
    {

        class RAMWin : public RAMBase< RAMWin >
        {
            using BASE = RAMBase< RAMWin >;
            friend BASE;

        public:
            RAMWin()
            {
                BASE::_name            = getName();
                BASE::_vendor          = getVendor();
                BASE::_serialNumber    = getSerialNumber();
                BASE::_model           = getModel();
                BASE::_total_Bytes     = getTotalSize_Bytes();
                BASE::_free_Bytes      = getFreeMemory();
                BASE::_available_Bytes = getFreeMemory();
            }

        private:
            // _____________________________________________________________________________________________________________________
            static std::string getVendor()
            {
                std::vector< const wchar_t* > names {};
                wmi::queryWMI("WIN32_PhysicalMemory", "Manufacturer", names);
                if (names.empty()) {
                    return "<unknown>";
                }
                auto ret = names[0];
                if (!ret) {
                    return "<unknown>";
                }
                return utils::wstring_to_std_string(ret);
            }

            // _____________________________________________________________________________________________________________________
            static std::string getName()
            {
                std::vector< const wchar_t* > names {};
                wmi::queryWMI("WIN32_PhysicalMemory", "Name", names);
                if (names.empty()) {
                    return "<unknown>";
                }
                auto ret = names[0];
                if (!ret) {
                    return "<unknown>";
                }
                return utils::wstring_to_std_string(ret);
            }

            // _____________________________________________________________________________________________________________________
            static std::string getModel()
            {
                std::vector< const wchar_t* > names {};
                wmi::queryWMI("WIN32_PhysicalMemory", "PartNumber", names);
                if (names.empty()) {
                    return "<unknown>";
                }
                auto ret = names[0];
                if (!ret) {
                    return "<unknown>";
                }
                return utils::wstring_to_std_string(ret);
            }

            // _____________________________________________________________________________________________________________________
            static std::string getSerialNumber()
            {
                std::vector< const wchar_t* > names {};
                wmi::queryWMI("WIN32_PhysicalMemory", "SerialNumber", names);
                if (names.empty()) {
                    return "<unknown>";
                }
                auto ret = names[0];
                if (!ret) {
                    return "<unknown>";
                }
                std::wstring tmp(ret);
                return utils::wstring_to_std_string(ret);
            }

            // _____________________________________________________________________________________________________________________
            static int64_t getTotalSize_Bytes()
            {
                MEMORYSTATUSEX status;
                status.dwLength = sizeof(status);
                GlobalMemoryStatusEx(&status);
                return static_cast< int64_t >(status.ullTotalPhys);
            }

            // _____________________________________________________________________________________________________________________
            static int64_t getFreeMemory()
            {
                // it will return L"FreePhysicalMemory" Str
                std::vector< wchar_t* > memories {};
                // Number of kilobytes of physical memory currently unused and available.
                wmi::queryWMI("CIM_OperatingSystem", "FreePhysicalMemory", memories);
                if (!memories.empty()) {
                    if (memories.front() == nullptr) {
                        return -1;
                    }
                    // keep it as totalSize_Bytes
                    return std::stoll(utils::wstring_to_std_string(memories[0])) * 1024;
                }
                return -1;
            }
        };

    }    // namespace detail

    using RAM = detail::RAMWin;

}    // namespace hwinfo
