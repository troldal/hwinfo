//
// Created by kenne on 28/10/2023.
//

#pragma once

#include <cstdint>
#include <string>

namespace hwinfo::WMI
{
    /**
     * @struct BatteryInfo
     * @brief Represents battery information obtained from WMI.
     */
    struct BatteryInfo
    {
        struct BatteryInfoType
        {
        };

        /**
         * @struct NAME
         * @brief Encapsulates the Name property of Win32_Battery.
         */
        struct NAME
        {
            using info_type   = BatteryInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Name";             ///< WMI field for the battery name.
        };

        /**
         * @struct DESCRIPTION
         * @brief Encapsulates the Description property of Win32_Battery.
         */
        struct DESCRIPTION
        {
            using info_type   = BatteryInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Description";      ///< WMI field for the battery description.
        };

        /**
         * @enum Chemistry
         * @brief Enumerates the types of battery chemistries.
         */
        enum class Chemistry : uint16_t {
            Other              = 1,
            Unknown            = 2,
            LeadAcid           = 3,
            NickelCadmium      = 4,
            NickelMetalHydride = 5,
            LithiumIon         = 6,
            ZincAir            = 7,
            LithiumPolymer     = 8
            // Add other battery chemistries as needed
        };

        /**
         * @struct CHEMISTRY
         * @brief Encapsulates the Chemistry property of Win32_Battery.
         */
        struct CHEMISTRY
        {
            using info_type   = BatteryInfoType;
            using value_type  = uint16_t;     ///< Type of the value being used.
            using result_type = Chemistry;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Chemistry";        ///< WMI field for the battery chemistry.
        };

        /**
         * @enum Status
         * @brief Enumerates the possible statuses of a battery.
         */
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

        /**
         * @struct STATUS
         * @brief Encapsulates the Status property of Win32_Battery.
         */
        struct STATUS
        {
            using info_type   = BatteryInfoType;
            using value_type  = uint16_t;    ///< Type of the value being used.
            using result_type = Status;      ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"BatteryStatus";    ///< WMI field for the battery status.
        };

        /**
         * @struct HEALTH
         * @brief Encapsulates the Health property of Win32_Battery.
         */
        struct HEALTH
        {
            using info_type   = BatteryInfoType;
            using value_type  = std::string;    ///< Type of the value being used.
            using result_type = std::string;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";    ///< WMI class name.
            static const inline std::wstring wmi_field = L"Status";           ///< WMI field for the battery health.
        };

        /**
         * @struct CAPACITY
         * @brief Encapsulates the Capacity property of Win32_Battery.
         */
        struct CAPACITY
        {
            using info_type   = BatteryInfoType;
            using value_type  = uint16_t;    ///< Type of the value being used.
            using result_type = uint16_t;    ///< Resultant type after conversion.

            static const inline std::wstring wmi_class = L"Win32_Battery";               ///< WMI class name.
            static const inline std::wstring wmi_field = L"EstimatedChargeRemaining";    ///< WMI field for the battery capacity.
        };
    };

    /**
     * @brief Converts a battery chemistry enum value to its string representation.
     * @param chemistry The battery chemistry.
     * @return A string representation of the battery chemistry.
     */
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

    /**
     * @brief Converts a battery status enum value to its string representation.
     * @param status The battery status.
     * @return A string representation of the battery status.
     */
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
