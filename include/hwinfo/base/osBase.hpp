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

namespace hwinfo
{
    enum class Architecture {
        x86,      // 32-bit
        x64,      // 64-bit
        arm,      // 32-bit
        arm64,    // 64-bit
        ia64,     // 64-bit
        Unknown
    };

    enum class Endianness { Big, Little, Unknown };

    std::string to_string(Architecture arch)
    {
        switch (arch) {
            case Architecture::x86:
                return "x86";
            case Architecture::x64:
                return "x64";
            case Architecture::arm:
                return "arm";
            case Architecture::arm64:
                return "arm64";
            case Architecture::ia64:
                return "ia64";
            default:
                return "Unknown";
        }
    }

    std::string to_string(Endianness endianness)
    {
        switch (endianness) {
            case Endianness::Big:
                return "Big";
            case Endianness::Little:
                return "Little";
            default:
                return "Unknown";
        }
    }
}    // namespace hwinfo

namespace hwinfo::detail
{
    template< typename IMPL >
    class OSBase
    {
        friend IMPL;

        struct OSItem
        {
            std::string fullName {};
            std::string name {};
            std::string version {};
            std::string kernel {};
            size_t       pointerSize { 0 };
            Architecture arch       = Architecture::Unknown;
            Endianness   endianness = Endianness::Unknown;
        };

    public:
        std::string fullName() { return IMPL::getFullName(); }

        std::string name() { return IMPL::getName(); }

        std::string version() { return IMPL::getVersion(); }

        std::string kernel() { return IMPL::getKernel(); }

        Architecture architecture() { return _item.arch; }

        size_t pointerSize() { return _item.pointerSize; }

        [[nodiscard]]
        bool isBigEndian() const
        {
            return _item._bigEndian;
        }
        [[nodiscard]]
        bool isLittleEndian() const
        {
            return _item._littleEndian;
        }

        /**
         * @brief Retrieves a string containing a report of the OS information.
         * @return A string containing a report of the OS information.
         */
        [[nodiscard]]
        std::string report() const
        {
            std::stringstream ss;
            ss << "OS Information:\n";
            ss << "\tFull Name: " << _item.fullName << "\n";
            ss << "\tName: " << _item.name << "\n";
            ss << "\tVersion: " << _item.version << "\n";
            ss << "\tKernel: " << _item.kernel << "\n";
            ss << "\tPointer Size: " << _item.pointerSize << "\n";
            ss << "\tArchitecture: " << to_string(_item.arch) << "\n";
            ss << "\tEndianness: " << to_string(_item.endianness) << "\n\n";
            return ss.str();
        }

        /**
         * @brief Overloaded ostream operator for printing the OS information.
         * @param os The ostream object.
         * @param osBase The OSBase object.
         * @return A reference to the ostream object.
         */
        friend std::ostream& operator<<(std::ostream& os, const OSBase& osBase) { return os << osBase.report(); }

        static IMPL getOSInfo() { return IMPL::getAllOSs(); }

    protected:
        ~OSBase() = default;

    private:
        OSBase() = default;

        static size_t getPointerSize() { return sizeof(void*) * 8; }

        static Endianness getEndianness()
        {
            char16_t dummy = 0x0102;
            return ((char*)&dummy)[0] == 0x01 ? Endianness::Big : Endianness::Little;
        }

        /**
         * @brief Provides access to the implementation-specific methods in the derived class.
         *
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the implementation-specific methods in the derived class.
         *
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        OSItem _item {};
    };

}    // namespace hwinfo::detail
