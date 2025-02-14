// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#pragma once

#include "../platform.h"

#ifdef HWINFO_WINDOWS

#include "../utils/stringutils.h"
#include "utils/wmi_wrapper.h"

namespace hwinfo {

// _____________________________________________________________________________________________________________________
std::vector<Disk> getAllDisks() {
  utils::WMI::_WMI wmi;
  const std::wstring query_string(
      L"SELECT Model, Manufacturer, SerialNumber, Size "
      L"FROM Win32_DiskDrive");
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }
  std::vector<Disk> disks;

  ULONG u_return = 0;
  IWbemClassObject* obj = nullptr;
  int disk_id = 0;
  while (wmi.enumerator) {
    wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (!u_return) {
      break;
    }
    Disk disk;
    disk._id = disk_id++;
    VARIANT vt_prop;
    obj->Get(L"Model", 0, &vt_prop, nullptr, nullptr);
    disk._model = utils::wstring_to_std_string(vt_prop.bstrVal);
    obj->Get(L"Manufacturer", 0, &vt_prop, nullptr, nullptr);
    disk._vendor = utils::wstring_to_std_string(vt_prop.bstrVal);
    obj->Get(L"SerialNumber", 0, &vt_prop, nullptr, nullptr);
    disk._serialNumber = utils::wstring_to_std_string(vt_prop.bstrVal);
    obj->Get(L"Size", 0, &vt_prop, nullptr, nullptr);
    disk._size_Bytes = static_cast<int64_t>(vt_prop.ullVal);
    VariantClear(&vt_prop);
    obj->Release();
    disks.push_back(std::move(disk));
  }
  return disks;
}

}  // namespace hwinfo

#endif  // HWINFO_WINDOWS