// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#include "hwinfo/hwinfo.hpp"
#include <hwinfo/PCIMapper.hpp>

#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "hwinfo is an open source, MIT licensed project that implements a platform independent "
              << "hardware and system information gathering API for C++.\n\n"
              << "If you face any issues, find bugs or if your platform is not supported yet, do not hesitate to create "
              << "a ticket at https://github.com/lfreist/hwinfo/issues.\n\n"
              << "Thanks for using hwinfo!" << std::endl;
    std::cout << std::endl << "Hardware Report:" << std::endl << std::endl;
    std::cout << "----------------------------------- CPU -----------------------------------" << std::endl;
    auto cpus = hwinfo::getCpuInfo();
    if (!cpus.items().empty()) std::cout << cpus;
    else
        std::cout << "CPU info could not be retreived" << std::endl;

    hwinfo::OS os = hwinfo::getOSInfo();
    std::cout << "----------------------------------- OS ------------------------------------" << std::endl;
    std::cout << os;


    auto gpus = hwinfo::getGpuInfo();
    std::cout << "----------------------------------- GPU -----------------------------------" << std::endl;
    if (!gpus.items().empty()) std::cout << gpus;
    else
        std::cout << "GPU info could not be retreived" << std::endl;

    auto ram = hwinfo::getRamInfo();
    std::cout << "----------------------------------- RAM -----------------------------------" << std::endl;
    if (!ram.items().empty()) std::cout << ram;
    else
        std::cout << "RAM info could not be retreived" << std::endl;

    auto main_board = hwinfo::getBaseboardInfo();
    std::cout << "------------------------------- Main Board --------------------------------" << std::endl;
    std::cout << main_board;

    std::cout << "------------------------------- Batteries ---------------------------------" << std::endl;
    hwinfo::Battery batteries = hwinfo::getBatteryInfo();
    if (!batteries.items().empty()) std::cout << batteries;
    else
        std::cout << "No Batteries installed or detected" << std::endl;

    std::cout << "--------------------------------- Disks -----------------------------------" << std::endl;
    hwinfo::Disk disks = hwinfo::getDiskInfo();
    if (!disks.items().empty()) std::cout << disks;
    else
        std::cout << "No Disks installed or detected" << std::endl;
}