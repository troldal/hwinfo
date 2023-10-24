// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::detail
{
    template< typename IMPL >
    class RAMBase
    {
        friend IMPL;

    public:
        RAMBase() = default;

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
        std::string model() const
        {
            return impl().getModel();
        }

        [[nodiscard]]
        std::string serialNumber() const
        {
            return impl().getSerialNumber();
        }

        [[nodiscard]]
        int64_t total_Bytes() const
        {
            return impl().getTotalMemory();
        }

        [[nodiscard]]
        int64_t free_Bytes() const
        {
            return impl().getFreeMemory();
        }

        [[nodiscard]]
        int64_t available_Bytes() const
        {
            return impl().getAvailableMemory();
        }

    protected:
        ~RAMBase() = default;

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

        std::string _vendor {};
        std::string _name {};
        std::string _model {};
        std::string _serialNumber {};
        int64_t     _total_Bytes     = -1;
        int64_t     _free_Bytes      = -1;
        int64_t     _available_Bytes = -1;
        int         _frequency       = -1;
    };

}    // namespace hwinfo::detail
