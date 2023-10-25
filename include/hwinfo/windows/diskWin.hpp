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

#include "../base/diskBase.hpp"
#include "utils/WMIWrapper.hpp"

namespace hwinfo
{

    namespace detail
    {

        class DiskWin : public DiskBase< DiskWin >
        {
            using BASE = DiskBase< DiskWin >;
            friend BASE;

        public:
            DiskWin() = default;

        private:
            [[nodiscard]]
            std::string getVendor() const
            {
                return BASE::_vendor;
            }

            [[nodiscard]]
            std::string getModel() const
            {
                return BASE::_model;
            }

            [[nodiscard]]
            std::string getSerialNumber() const
            {
                return BASE::_serialNumber;
            }

            [[nodiscard]]
            int64_t getSize() const
            {
                return BASE::_size_Bytes;
            }

            [[nodiscard]]
            int getId() const
            {
                return BASE::_id;
            }

            static std::vector< DiskWin > getAllDisks_impl()
            {
                auto               wmi = utils::WMI::WMIWrapper();
                const std::wstring query_string(L"SELECT Model, Manufacturer, SerialNumber, Size "
                                                L"FROM Win32_DiskDrive");
                bool               success = wmi.execute_query(query_string);
                if (!success) {
                    return {};
                }
                std::vector< DiskWin > disks;

                ULONG             u_return = 0;
                IWbemClassObject* obj      = nullptr;
                int               disk_id  = 0;
                while (wmi.m_enumerator) {
                    wmi.m_enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                    if (!u_return) {
                        break;
                    }
                    DiskWin disk;
                    disk._id = disk_id++;
                    VARIANT vt_prop;
                    obj->Get(L"Model", 0, &vt_prop, nullptr, nullptr);
                    disk._model = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"Manufacturer", 0, &vt_prop, nullptr, nullptr);
                    disk._vendor = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"SerialNumber", 0, &vt_prop, nullptr, nullptr);
                    disk._serialNumber = utils::wstring_to_std_string(vt_prop.bstrVal);
                    obj->Get(L"Size", 0, &vt_prop, nullptr, nullptr);
                    disk._size_Bytes = static_cast< int64_t >(vt_prop.ullVal);
                    VariantClear(&vt_prop);
                    obj->Release();
                    disks.push_back(std::move(disk));
                }
                return disks;
            }
        };

    }

    using Disk = detail::DiskWin;

}    // namespace hwinfo
