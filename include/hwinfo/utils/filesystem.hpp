#pragma once

#include <string>

#include "../cpu.hpp"

namespace hwinfo {
namespace filesystem {

bool exists(const std::string& path);

std::vector<std::string> getDirectoryEntries(const std::string& path);

#if defined(HWINFO_UNIX) || defined(HWINFO_APPLE)
int64_t get_specs_by_file_path(const std::string& path);
#endif  // HWINFO_UNIX || HWINFO_APPLE

}  // namespace filesystem
}  // namespace hwinfo
