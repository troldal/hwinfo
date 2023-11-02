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
 * @file cpuBase.hpp
 * @brief This file contains the base class for retrieving CPU information using CRTP.
 */

#pragma once

#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace hwinfo::detail
{
    /**
     * @class CPUBase
     * @brief A Curiously Recurring Template Pattern (CRTP) base class for retrieving CPU information.
     * @tparam IMPL The derived class that inherits from CPUBase.
     *
     * This class provides a common interface and data structure to represent CPU information. The derived class
     * should provide the implementation specific to the platform.
     */
    template< typename IMPL >
    class CPUBase
    {
        friend IMPL;    ///< Allow the derived class to access private members.

        /**
         * @struct CpuItem
         * @brief Represents a single CPU's information.
         */
        struct CpuItem
        {
            std::string                vendor {};                   ///< The CPU's vendor.
            std::string                modelName {};                ///< The model name of the CPU.
            int64_t                    L1CacheSize { -1 };          ///< The size of the L1 cache in KB. -1 if unknown.
            int64_t                    L2CacheSize { -1 };          ///< The size of the L2 cache in KB. -1 if unknown.
            int64_t                    L3CacheSize { -1 };          ///< The size of the L3 cache in KB. -1 if unknown.
            uint32_t                   numPhysicalCores { 0 };      ///< The number of physical cores.
            uint32_t                   numLogicalCores { 0 };       ///< The number of logical cores (including hyper-threaded cores).
            int64_t                    maxClockSpeed { -1 };        ///< The maximum clock speed in MHz.
            int64_t                    regularClockSpeed { -1 };    ///< The regular clock speed in MHz.
            std::vector< std::string > flags {};                    ///< A list of flags or features supported by the CPU.
        };

    public:
        /**
         * @brief Retrieves the list of CPU items.
         * @return A const reference to a vector of CpuItem structures.
         */
        [[nodiscard]]
        std::vector< CpuItem > const& items() const
        {
            return _items;
        }

        /**
         * @brief Retrieves the number of CPU items.
         * @return The number of CPU items.
         */
        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        /**
         * @brief Calculates the total number of physical cores.
         * @return The total number of physical cores across all CPU items.
         */
        [[nodiscard]]
        auto physicalCoreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0),
                                   [](uint32_t sum, const CpuItem& item) { return sum + item.numPhysicalCores; });
        }

        /**
         * @brief Calculates the total number of logical cores.
         * @return The total number of logical cores across all CPU items.
         */
        [[nodiscard]]
        auto logicalCoreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0),
                                   [](uint32_t sum, const CpuItem& item) { return sum + item.numLogicalCores; });
        }

        /**
         * @brief Retrieves the current clock speed of a specific thread.
         * @param thread_id The index of the thread.
         * @return The current clock speed in MHz.
         */
        [[nodiscard]]
        int64_t currentClockSpeed(int thread_id) const
        {
            return impl().getCurrentClockSpeed(thread_id);
        }

        /**
         * @brief Retrieves the current clock speeds of all threads.
         * @return A vector of current clock speeds in MHz.
         */
        [[nodiscard]]
        std::vector< int64_t > currentClockSpeed() const
        {
            return impl().getCurrentClockSpeed();
        }

        /**
         * @brief Calculates the average CPU utilization across all threads.
         * @return The average CPU utilization as a percentage.
         */
        [[nodiscard]]
        double currentUtilisation() const
        {
            return impl().getCurrentUtilisation();
        }

        /**
         * @brief Retrieves the CPU utilization of a specific thread.
         * @param thread_index The index of the thread.
         * @return The CPU utilization as a percentage.
         */
        [[nodiscard]]
        double threadUtilisation(int thread_index) const
        {
            return impl().getThreadUtilisation(thread_index);
        }

        /**
         * @brief Retrieves the CPU utilization of all threads.
         * @return A vector of CPU utilizations as percentages.
         */
        [[nodiscard]]
        std::vector< double > threadsUtilisation() const
        {
            return impl().getThreadsUtilisation();
        }

        /**
         * @brief Generates a report of the CPU information.
         * @return A string containing the CPU information.
         */
        [[nodiscard]]
        std::string report() const
        {
            std::stringstream reportStream;
            size_t            index = 1;
            for (const auto& cpu : items()) {
                reportStream << "CPU " << index++ << " Information:\n";
                reportStream << "\tVendor: " << cpu.vendor << "\n";
                reportStream << "\tModel: " << cpu.modelName << "\n";
                reportStream << "\tClock Speed: " << cpu.maxClockSpeed << " MHz\n";
                reportStream << "\tNumber of Cores: " << cpu.numPhysicalCores << "\n";
                reportStream << "\tNumber of Threads: " << cpu.numLogicalCores << "\n";
                reportStream << "\tL1 Cache Size: " << cpu.L1CacheSize << " KB\n";
                reportStream << "\tL2 Cache Size: " << cpu.L2CacheSize << " KB\n";
                reportStream << "\tL3 Cache Size: " << cpu.L3CacheSize << " KB\n\n";
            }
            return reportStream.str();
        }

        /**
         * @brief Overloaded stream operator to print CPU information.
         * @param os The output stream to write to.
         * @param cpuBase The CPUBase instance to print.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const CPUBase& cpuBase) { return os << cpuBase.report(); }

        /**
         * @brief Factory function to create and retrieve an instance of the derived class.
         * @return An instance of the derived class.
         */
        [[nodiscard]]
        static IMPL getCpuInfo()
        {
            return IMPL::getAllCPUs();
        }

    protected:
        ~CPUBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        CPUBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Adds a CpuItem to the internal list.
         * @param item The CpuItem to add.
         */
        void addItem(const CpuItem& item) { _items.push_back(item); }

        /**
         * @brief Casts the instance to the derived type.
         * @return A reference to the derived instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Casts the instance to the derived type.
         * @return A const reference to the derived instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        std::vector< CpuItem > _items {};    ///< Internal storage of CPU information.
    };
}    // namespace hwinfo::detail
