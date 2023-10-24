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

#include "../base/os.h"
#include <sys/stat.h>
#include <sys/utsname.h>

namespace hwinfo
{

    namespace detail
    {

        class OSLinux : public OSBase< OSLinux >
        {
            using BASE = OSBase< OSLinux >;
            friend BASE;

        public:
            OSLinux()
            {
                BASE::_32bit        = getIs32bit();
                BASE::_64bit        = getIs64bit();
                BASE::_bigEndian    = getIsBigEndian();
                BASE::_littleEndian = !_bigEndian;
            }

        private:
            static std::string getFullName()
            {
                std::string   line;
                std::ifstream stream("/etc/os-release");
                if (!stream) {
                    return "Linux <unknown version>";
                }
                while (getline(stream, line)) {
                    if (utils::starts_with(line, "PRETTY_NAME")) {
                        line = line.substr(line.find('=') + 1, line.length());
                        // remove \" at begin and end of the substring result
                        return { line.begin() + 1, line.end() - 1 };
                    }
                }
                stream.close();
                return "Linux <unknown version>";
            }

            static std::string getName()
            {
                std::string   line;
                std::ifstream stream("/etc/os-release");
                if (!stream) {
                    return "Linux";
                }
                while (getline(stream, line)) {
                    if (utils::starts_with(line, "NAME")) {
                        line = line.substr(line.find('=') + 1, line.length());
                        // remove \" at begin and end of the substring result
                        return { line.begin() + 1, line.end() - 1 };
                    }
                }
                stream.close();
                return "Linux";
            }

            static std::string getVersion()
            {
                std::string   line;
                std::ifstream stream("/etc/os-release");
                if (!stream) {
                    return "<unknown version>";
                }
                while (getline(stream, line)) {
                    if (utils::starts_with(line, "VERSION_ID")) {
                        line = line.substr(line.find('=') + 1, line.length());
                        // remove \" at begin and end of the substring result
                        return { line.begin() + 1, line.end() - 1 };
                    }
                }
                stream.close();
                return "<unknown version>";
            }

            static std::string getKernel()
            {
                static utsname info;
                if (uname(&info) == 0) {
                    return info.release;
                }
                return "<unknown kernel>";
            }

            static bool getIs64bit()
            {
                struct stat buffer
                {
                };
                return (stat("/lib64/ld-linux-x86-64.so.2", &buffer) == 0);
            }

            static bool getIs32bit() { return !getIs64bit(); }

            static bool getIsBigEndian()
            {
                char16_t dummy = 0x0102;
                return ((char*)&dummy)[0] == 0x01;
            }

            static bool getIsLittleEndian()
            {
                char16_t dummy = 0x0102;
                return ((char*)&dummy)[0] == 0x02;
            }
        };

    }    // namespace detail

    using OS = detail::OSLinux;

}    // namespace hwinfo
