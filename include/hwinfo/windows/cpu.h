// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

#pragma once

#include "../base/cpu.h"
#include "utils/wmi_wrapper.h"

namespace hwinfo
{

    class CPUWin : public CPUBase< CPUWin >
    {
        using BASE = CPUBase< CPUWin >;

    public:
        [[nodiscard]]
        int getId() const
        {
            return BASE::_id;
        }

        [[nodiscard]]
        const std::string& getModelName() const
        {
            return BASE::_modelName;
        }

        [[nodiscard]]
        const std::string& getVendor() const
        {
            return BASE::_vendor;
        }

        [[nodiscard]]
        int64_t getL1CacheSize() const
        {
            return BASE::_L1CacheSize_Bytes;
        }

        [[nodiscard]]
        int64_t getL2CacheSize() const
        {
            return BASE::_L2CacheSize_Bytes;
        }

        [[nodiscard]]
        int64_t getL3CacheSize() const
        {
            return BASE::_L3CacheSize_Bytes;
        }

        [[nodiscard]]
        int getNumPhysicalCores() const
        {
            return BASE::_numPhysicalCores;
        }

        [[nodiscard]]
        int getNumLogicalCores() const
        {
            return BASE::_numLogicalCores;
        }

        [[nodiscard]]
        int64_t getMaxClockSpeed() const
        {
            return BASE::_maxClockSpeed_MHz;
        }

        [[nodiscard]]
        int64_t getRegularClockSpeed() const
        {
            return BASE::_regularClockSpeed_MHz;
        }

        [[nodiscard]]
        int64_t getCurrentClockSpeed(int thread_id) const
        {
            auto data =
                utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation", L"PercentProcessorPerformance");
            if (data.empty()) {
                return -1;
            }
            double performance = std::stod(data[thread_id]) / 100;
            return static_cast< int64_t >(static_cast< double >(_maxClockSpeed_MHz) * performance);
        }

        [[nodiscard]]
        std::vector< int64_t > getCurrentClockSpeed() const
        {
            std::vector< int64_t > result;
            result.reserve(_numLogicalCores);
            auto data =
                utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation", L"PercentProcessorPerformance");
            if (data.empty()) {
                result.resize(_numLogicalCores, -1);
                return result;
            }
            for (auto& v : data) {
                double performance = std::stod(v) / 100;
                result.push_back(static_cast< int64_t >(static_cast< double >(_maxClockSpeed_MHz) * performance));
            }
            return result;
        }

        [[nodiscard]]
        double getCurrentUtilisation() const
        {
            std::vector< bstr_t > percentage {};
            const std::string&    query =
                "Win32_PerfFormattedData_Counters_ProcessorInformation WHERE Name='" + std::to_string(0) + ",_Total'";
            wmi::queryWMI(query, "PercentProcessorUtility", percentage);
            if (percentage.empty()) {
                return -1.0;
            }

            const char* strValue = static_cast< const char* >(percentage[0]);
            return std::stod(strValue);
        }

        [[nodiscard]]
        double getThreadUtilisation(int thread_id) const
        {
            auto data =
                utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation", L"PercentProcessorUtility");
            if (data.empty()) {
                return -1.f;
            }
            std::string thread_value = data[thread_id];
            if (thread_value.empty()) {
                return -1.f;
            }
            return std::stod(thread_value);
        }

        [[nodiscard]]
        std::vector< double > getThreadsUtilisation() const
        {
            std::vector< double > thread_utility;
            thread_utility.reserve(_numLogicalCores);
            auto data =
                utils::WMI::query< std::string >(L"Win32_PerfFormattedData_Counters_ProcessorInformation", L"PercentProcessorUtility");
            if (data.empty()) {
                thread_utility.resize(_numLogicalCores, -1.f);
                return thread_utility;
            }
            for (const auto& v : data) {
                if (v.empty()) {
                    thread_utility.push_back(-1.f);
                }
                else {
                    thread_utility.push_back(std::stod(v) / 100.f);
                }
            }
            return thread_utility;
        }

        [[nodiscard]]
        const std::vector< std::string >& getFlags() const
        {
            return BASE::_flags;
        }

        static std::vector< CPUWin > getAllCPUs_impl()
        {
            utils::WMI::_WMI   wmi;
            const std::wstring query_string(
                L"SELECT Name, Manufacturer, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed, L2CacheSize, L3CacheSize "
                L"FROM Win32_Processor");
            bool success = wmi.execute_query(query_string);
            if (!success) {
                return {};
            }
            std::vector< CPUWin > cpus;

            ULONG             u_return = 0;
            IWbemClassObject* obj      = nullptr;
            int               cpu_id   = 0;
            while (wmi.enumerator) {
                wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
                if (!u_return) {
                    break;
                }
                CPUWin cpu;
                cpu._id = cpu_id++;
                VARIANT vt_prop;

                obj->Get(L"Name", 0, &vt_prop, nullptr, nullptr);
                cpu._modelName = utils::wstring_to_std_string(vt_prop.bstrVal);

                obj->Get(L"Manufacturer", 0, &vt_prop, nullptr, nullptr);
                cpu._vendor = utils::wstring_to_std_string(vt_prop.bstrVal);

                obj->Get(L"NumberOfCores", 0, &vt_prop, nullptr, nullptr);
                cpu._numPhysicalCores = vt_prop.intVal;

                obj->Get(L"NumberOfLogicalProcessors", 0, &vt_prop, nullptr, nullptr);
                cpu._numLogicalCores = vt_prop.intVal;

                obj->Get(L"MaxClockSpeed", 0, &vt_prop, nullptr, nullptr);
                cpu._maxClockSpeed_MHz     = vt_prop.uintVal;

                cpu._regularClockSpeed_MHz = vt_prop.uintVal;

                obj->Get(L"L2CacheSize", 0, &vt_prop, nullptr, nullptr);
                cpu._L2CacheSize_Bytes = vt_prop.uintVal;

                obj->Get(L"L3CacheSize", 0, &vt_prop, nullptr, nullptr);
                cpu._L3CacheSize_Bytes = vt_prop.uintVal;

                VariantClear(&vt_prop);
                obj->Release();
                cpus.push_back(std::move(cpu));
            }
            return cpus;
        }
    };

    using CPU = CPUWin;

}    // namespace hwinfo
