#include <fstream>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "battery_monitor.hpp"

std::ifstream batfile;
std::ifstream adptfile;
uint16_t charge;

uint16_t BatteryCharge() noexcept
{
    if (batfile.good())
    {
        batfile.seekg(0);
        batfile >> charge;
    }
    else
    {
        batfile.open(BatteryPath, std::ifstream::in);
        charge = TripPoint;
    }
    return charge;
}

bool AdapterOnline() noexcept
{
    if (adptfile.good())
    {
        bool online;
        adptfile.seekg(0);
        adptfile >> online;
        return online;
    }
    else
    {
        adptfile.open(AdapterPath, std::ifstream::in);
        return false;
    }
}

int main()
{
    batfile.open(BatteryPath, std::ifstream::in);
    adptfile.open(AdapterPath, std::ifstream::in);

    while (BatteryCharge() > TripPoint || AdapterOnline())
        std::this_thread::sleep_for(std::chrono::seconds(charge*ChargeMultiplier));

    while (system(TriggerCommand))
        std::this_thread::sleep_for(std::chrono::seconds(charge));

    return 0;    
}
