//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    struct BatteryInfo
    {
        struct NAME
        {
            using value_type  = std::string;
            using result_type = std::string;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"Name";
        };

        struct DESCRIPTION
        {
            using value_type  = std::string;
            using result_type = std::string;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"Description";
        };

        enum class Chemistry : uint16_t {
            Other              = 1,
            Unknown            = 2,
            LeadAcid           = 3,
            NickelCadmium      = 4,
            NickelMetalHydride = 5,
            LithiumIon         = 6,
            ZincAir            = 7,
            LithiumPolymer     = 8
        };

        struct CHEMISTRY
        {
            using value_type  = uint16_t;
            using result_type = Chemistry;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"Chemistry";
        };

        enum class Status : uint16_t {
            Discharging         = 1,
            OnPower             = 2,
            FullyCharged        = 3,
            Low                 = 4,
            Critical            = 5,
            Charging            = 6,
            ChargingAndLow      = 7,
            ChargingAndCritical = 8,
            Undefined           = 9,
            PartiallyCharged    = 10
        };

        struct STATUS
        {
            using value_type  = uint16_t;
            using result_type = Status;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"BatteryStatus";
        };

        struct HEALTH
        {
            using value_type  = std::string;
            using result_type = std::string;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"Status";
        };

        struct CAPACITY
        {
            using value_type  = uint16_t;
            using result_type = uint16_t;

            static const inline std::wstring wmi_class = L"Win32_Battery";
            static const inline std::wstring wmi_field = L"EstimatedChargeRemaining";
        };
    };

    std::string to_string(const BatteryInfo::Chemistry& chemistry)
    {
        switch (chemistry) {
            case BatteryInfo::Chemistry::Other:
                return "Other";
            case BatteryInfo::Chemistry::Unknown:
                return "Unknown";
            case BatteryInfo::Chemistry::LeadAcid:
                return "LeadAcid";
            case BatteryInfo::Chemistry::NickelCadmium:
                return "NickelCadmium";
            case BatteryInfo::Chemistry::NickelMetalHydride:
                return "NickelMetalHydride";
            case BatteryInfo::Chemistry::LithiumIon:
                return "LithiumIon";
            case BatteryInfo::Chemistry::ZincAir:
                return "ZincAir";
            case BatteryInfo::Chemistry::LithiumPolymer:
                return "LithiumPolymer";
            default:
                return "Unknown";
        }
    }

    std::string to_string(const BatteryInfo::Status& status)
    {
        switch (status) {
            case BatteryInfo::Status::Discharging:
                return "Discharging";
            case BatteryInfo::Status::OnPower:
                return "OnPower";
            case BatteryInfo::Status::FullyCharged:
                return "FullyCharged";
            case BatteryInfo::Status::Low:
                return "Low";
            case BatteryInfo::Status::Critical:
                return "Critical";
            case BatteryInfo::Status::Charging:
                return "Charging";
            case BatteryInfo::Status::ChargingAndLow:
                return "ChargingAndLow";
            case BatteryInfo::Status::ChargingAndCritical:
                return "ChargingAndCritical";
            case BatteryInfo::Status::Undefined:
                return "Undefined";
            case BatteryInfo::Status::PartiallyCharged:
                return "PartiallyCharged";
            default:
                return "Unknown";
        }
    }
}    // namespace hwinfo::WMI
