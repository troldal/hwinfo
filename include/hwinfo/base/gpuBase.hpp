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
 * @file GPUBase.hpp
 * @namespace hwinfo::detail
 * @brief Contains the definition of the GPUBase class template, serving as a CRTP base class for GPU information retrieval.
 */

#pragma once

#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace hwinfo::detail
{

    /**
     * @class GPUBase
     * @brief A Curiously Recurring Template Pattern (CRTP) base class for retrieving GPU information.
     *
     * This class template provides common functionality for retrieving and storing GPU information.
     * Derived classes should provide the actual implementation specific to different operating systems or libraries.
     * @tparam IMPL The type of the derived class implementing the GPU information retrieval.
     */
    template< typename IMPL >
    class GPUBase
    {
        friend IMPL;    ///< Grant access to the derived class for certain operations.

        /**
         * @struct GpuItem
         * @brief Represents information about a single GPU.
         */
        struct GpuItem
        {
            std::string vendor {};           ///< The GPU vendor.
            std::string name {};             ///< The name of the GPU.
            std::string driverVersion {};    ///< The version of the GPU driver.
            int64_t     memory { 0 };        ///< The amount of memory on the GPU in bytes.
            int64_t     frequency { 0 };     ///< The frequency of the GPU in Hz.
            int         num_cores { 0 };     ///< The number of cores in the GPU.
            int         id { 0 };            ///< A unique identifier for the GPU.
        };

    public:
        /**
         * @brief Retrieves the GPU information items.
         * @return A constant reference to a vector of GpuItem structures.
         * Each item in the vector represents information about a single GPU.
         */
        [[nodiscard]]
        std::vector< GpuItem > const& items() const
        {
            return _items;
        }

        /**
         * @brief Retrieves the number of GPU items.
         * @return The number of GPU items.
         */
        [[nodiscard]]
        size_t count() const
        {
            return _items.size();
        }

        /**
         * @brief Calculates the total number of GPU cores.
         * @return The total number of cores across all GPUs.
         */
        [[nodiscard]]
        auto coreCount() const
        {
            return std::accumulate(_items.begin(), _items.end(), uint32_t(0),
                                   [](uint32_t sum, const GpuItem& item) { return sum + item.num_cores; });
        }

        [[nodiscard]]
        std::string report() const
        {
            std::stringstream reportStream;

            reportStream << "Number of GPUs: " << count() << "\n\n";

            size_t index = 1;
            for (const auto& item : items()) {
                reportStream << "GPU " << index++ << ":\n";
                reportStream << "  Vendor: " << item.vendor << "\n";
                reportStream << "  Name: " << item.name << "\n";
                reportStream << "  Driver version: " << item.driverVersion << "\n";
                reportStream << "  Memory: " << item.memory << " bytes\n";
                reportStream << "  Frequency: " << item.frequency << " Hz\n";
                reportStream << "  Number of cores: " << item.num_cores << "\n";
                reportStream << "  ID: " << item.id << "\n\n";
            }

            return reportStream.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const GPUBase& gpu) { return os << gpu.report(); }

        /**
         * @brief Creates an instance of the derived class and retrieves GPU information.
         * @return An instance of the derived class populated with GPU information.
         */
        static IMPL getGpuInfo() { return IMPL::getAllGPUs(); }

    protected:
        ~GPUBase() = default;    ///< Defaulted virtual destructor for safe polymorphism.

    private:
        GPUBase() = default;    ///< Default constructor is private to ensure control of object creation.

        /**
         * @brief Adds a GPU information item to the list.
         * @param item The GPU information item to add.
         */
        void addItem(const GpuItem& item) { _items.push_back(item); }

        /**
         * @brief Provides non-const access to the derived class instance.
         * @return A reference to the derived class instance.
         */
        IMPL& impl() { return static_cast< IMPL& >(*this); }

        /**
         * @brief Provides const access to the derived class instance.
         * @return A const reference to the derived class instance.
         */
        IMPL const& impl() const { return static_cast< IMPL const& >(*this); }

        std::vector< GpuItem > _items {};    ///< The list of GPU information items.
    };

}    // namespace hwinfo::detail
