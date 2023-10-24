// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

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
                BASE::_name            = getName_s();
                BASE::_vendor          = getVendor_s();
                BASE::_serialNumber    = getSerialNumber_s();
                BASE::_model           = getModel_s();
                BASE::_total_Bytes     = getTotalSize_Bytes_s();
                BASE::_free_Bytes      = getFreeMemory_s();
                BASE::_available_Bytes = getFreeMemory_s();
            }

            [[nodiscard]]
            std::string getVendor() const
            {
                return BASE::_vendor;
            }

            [[nodiscard]]
            std::string getName() const
            {
                return BASE::_name;
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
            int64_t getTotalMemory() const
            {
                return BASE::_total_Bytes;
            }

            [[nodiscard]]
            int64_t getFreeMemory() const
            {
                return BASE::_free_Bytes;
            }

            [[nodiscard]]
            int64_t getAvailableMemory() const
            {
                return BASE::_available_Bytes;
            }

        private:
            // _____________________________________________________________________________________________________________________
            static std::string getVendor_s()
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
            static std::string getName_s()
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
            static std::string getModel_s()
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
            static std::string getSerialNumber_s()
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
            static int64_t getTotalSize_Bytes_s()
            {
                MEMORYSTATUSEX status;
                status.dwLength = sizeof(status);
                GlobalMemoryStatusEx(&status);
                return static_cast< int64_t >(status.ullTotalPhys);
            }

            // _____________________________________________________________________________________________________________________
            static int64_t getFreeMemory_s()
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
