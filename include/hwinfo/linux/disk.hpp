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

#include "../base/disk.hpp"

namespace hwinfo
{

    namespace detail
    {

        class DiskLinux : public DiskBase< DiskLinux >
        {
            using BASE = DiskBase< DiskLinux >;
            friend BASE;

        public:
            DiskLinux() = default;

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

            static std::vector< DiskLinux > getAllDisks_impl()
            {
                std::vector< DiskLinux > disks;
                const std::string        base_path("/sys/class/block/");
                for (const auto& entry : filesystem::getDirectoryEntries(base_path)) {
                    DiskLinux   disk;
                    std::string path(base_path + "/" + entry + "/device/");
                    if (!filesystem::exists(path)) {
                        continue;
                    }
                    std::ifstream f(path + "vendor");
                    if (f) {
                        getline(f, disk._vendor);
                    }
                    else {
                        disk._vendor = "<unknown>";
                    }
                    f.close();
                    f.open(path + "model");
                    if (f) {
                        getline(f, disk._model);
                    }
                    else {
                        disk._model = "<unknown>";
                    }
                    f.close();
                    f.open(path + "serial");
                    if (f) {
                        getline(f, disk._serialNumber);
                    }
                    else {
                        disk._serialNumber = "<unknown>";
                    }
                    f.close();
                    utils::strip(disk._vendor);
                    utils::strip(disk._model);
                    utils::strip(disk._serialNumber);
                    disk._size_Bytes = -1;
                    /*
                    struct statvfs buf {};
                    std::string mount_path("/dev/");
                    mount_path.append(split_get_index(entry.path().string(), "/", -1));
                    if (statvfs(mount_path.c_str(), &buf)) {
                      size = static_cast<int64_t>(buf.f_bsize * buf.f_bfree);
                    }
                    */

                    disks.push_back(std::move(disk));
                }
                return disks;
            }
        };

    }

    using Disk = detail::DiskLinux;

}    // namespace hwinfo
