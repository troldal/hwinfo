/*

     ad88888ba                           88                 ad88
    d8"     "8b                          88                d8"
    Y8,                                  88                88
    `Y8aaaaa,    8b       d8  ,adPPYba,  88  8b,dPPYba,  MM88MMM  ,adPPYba,
      `"""""8b,  `8b     d8'  I8[    ""  88  88P'   `"8a   88    a8"     "8a
            `8b   `8b   d8'    `"Y8ba,   88  88       88   88    8b       d8
    Y8a     a8P    `8b,d8'    aa    ]8I  88  88       88   88    "8a,   ,a8"
     "Y88888P"       Y88'     `"YbbdP"'  88  88       88   88     `"YbbdP"'
                     d8'
                    d8'

    Based on hwinfo by:
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

#include <iostream>
#include <sstream>
#include <string>

namespace hwinfo
{
    /**
     * @brief Enumeration representing different computer architectures.
     */
    enum class Architecture {
        x86,       ///< 32-bit x86 architecture
        x64,       ///< 64-bit x86_64 architecture
        arm,       ///< 32-bit ARM architecture
        arm64,     ///< 64-bit ARM architecture
        ia64,      ///< 64-bit Intel Itanium architecture
        Unknown    ///< Unknown architecture
    };

    /**
     * @brief Enumeration representing endianness of the system.
     */
    enum class Endianness {
        Big,       ///< Big-endian byte order
        Little,    ///< Little-endian byte order
        Unknown    ///< Unknown byte order
    };

    /**
     * @brief Converts an Architecture enumeration value to a string representation.
     * @param arch The architecture to convert.
     * @return The string representation of the architecture.
     */
    inline std::string to_string(Architecture arch)
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

    /**
     * @brief Converts an Endianness enumeration value to a string representation.
     * @param endianness The endianness to convert.
     * @return The string representation of the endianness.
     */
    inline static std::string to_string(Endianness endianness)
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

    namespace detail
    {
        /**
         * @brief Base class template for retrieving operating system information.
         * @tparam IMPL The derived implementation class.
         *
         * This class follows the Curiously Recurring Template Pattern (CRTP) to
         * allow for static polymorphism. It defines a common interface and some
         * utility functions for interacting with operating system information.
         */
        template< typename IMPL >
        class OSBase
        {
            friend IMPL;

            /**
             * @brief Internal structure to hold operating system information.
             */
            struct OSItem
            {
                std::string  name {};           ///< The short name of the OS.
                std::string  fullName {};       ///< The full name and version of the OS.
                std::string  version {};        ///< The version number of the OS.
                std::string  kernel {};         ///< The kernel version of the OS.
                size_t       pointerSize {};    ///< The pointer size of the OS (in bits).
                Architecture arch {};           ///< The architecture of the OS.
                Endianness   endianness {};     ///< The endianness of the OS.
            };

        public:
            /**
             * @brief Gets the short name of the operating system.
             * @return The short name of the OS.
             */
            std::string name() { return _item.name; }

            /**
             * @brief Gets the full name and version of the operating system.
             * @return The full name and version of the OS.
             */
            std::string fullName() { return _item.fullName; }

            /**
             * @brief Gets the version number of the operating system.
             * @return The version number of the OS.
             */
            std::string version() { return _item.version; }

            /**
             * @brief Gets the kernel version of the operating system.
             * @return The kernel version of the OS.
             */
            std::string kernel() { return _item.kernel; }

            /**
             * @brief Gets the pointer size of the operating system in bits.
             * @return The pointer size in bits.
             */
            size_t pointerSize() { return _item.pointerSize; }

            /**
             * @brief Gets the architecture of the operating system.
             * @return The architecture enumeration value.
             */
            Architecture architecture() { return _item.arch; }

            /**
             * @brief Gets the endianness of the operating system.
             * @return The endianness enumeration value.
             */
            Endianness endianness() { return _item.endianness; }

            /**
             * @brief Generates a report of the operating system information.
             * @return A string containing the formatted OS information.
             */
            [[nodiscard]]
            std::string report() const
            {
                std::stringstream ss;
                ss << "OS Information:\n";
                ss << "\tName: " << _item.name << "\n";
                ss << "\tFull Name: " << _item.fullName << "\n";
                ss << "\tVersion: " << _item.version << "\n";
                ss << "\tKernel: " << _item.kernel << "\n";
                ss << "\tPointer Size: " << _item.pointerSize << "\n";
                ss << "\tArchitecture: " << to_string(_item.arch) << "\n";
                ss << "\tEndianness: " << to_string(_item.endianness) << "\n\n";
                return ss.str();
            }

            /**
             * @brief Output stream operator overload for OSBase.
             * @param os The output stream to write to.
             * @param osBase The OSBase object to output.
             * @return The output stream.
             */
            friend std::ostream& operator<<(std::ostream& os, const OSBase& osBase) { return os << osBase.report(); }

            /**
             * @brief Retrieves the operating system information.
             * @return An instance of the derived implementation class.
             */
            static IMPL getOSInfo() { return IMPL::getAllOSs(); }

        protected:
            ~OSBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

        private:
            OSBase() = default;    ///< Default constructor is private to ensure control of object creation.

            /**
             * @brief Gets the pointer size of the operating system in bits.
             * @return The pointer size in bits.
             */
            static size_t getPointerSize() { return sizeof(void*) * 8; }

            /**
             * @brief Determines the endianness of the operating system.
             * @return The endianness enumeration value.
             */
            static Endianness getEndianness()
            {
                char16_t dummy = 0x0102;
                return ((char*)&dummy)[0] == 0x01 ? Endianness::Big : Endianness::Little;
            }

            /**
             * @brief Provides access to the derived implementation class.
             * @return A reference to the derived class.
             */
            IMPL& impl() { return static_cast< IMPL& >(*this); }

            /**
             * @brief Provides const access to the derived implementation class.
             * @return A const reference to the derived class.
             */
            IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

            OSItem _item {};    ///< Internal storage of OS information.
        };

    }    // namespace detail

}    // namespace hwinfo
