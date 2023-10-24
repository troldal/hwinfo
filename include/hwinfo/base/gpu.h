// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace hwinfo::detail
{

    template< typename IMPL >
    class GPUBase
    {

        friend IMPL;

    public:

        [[nodiscard]]
        std::string vendor() const
        {
            return impl().getVendor();
        }

        [[nodiscard]]
        std::string name() const
        {
            return impl().getName();
        }

        [[nodiscard]]
        std::string driverVersion() const
        {
            return impl().getDriverVersion();
        }

        [[nodiscard]]
        int64_t memory_Bytes() const
        {
            return impl().getMemory();
        }

        [[nodiscard]]
        int64_t frequency_MHz() const
        {
            return impl().getFrequency();
        }

        [[nodiscard]]
        int num_cores() const
        {
            return impl().getNumCores();
        }

        [[nodiscard]]
        int id() const
        {
            return impl().getId();
        }

        static std::vector< IMPL > getAllGPUs() { return IMPL::getAllGPUs_impl(); }

    protected:
        ~GPUBase() = default;

    private:
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

        GPUBase() = default;
        std::string _vendor {};
        std::string _name {};
        std::string _driverVersion {};
        int64_t     _memory_Bytes { 0 };
        int64_t     _frequency_MHz { 0 };
        int         _num_cores { 0 };
        int         _id { 0 };

        std::string _vendor_id {};
        std::string _device_id {};
    };


}    // namespace hwinfo::detail
