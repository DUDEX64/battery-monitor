#include <fstream>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "battery_monitor.hpp"

std::ifstream batfile;
std::ifstream adptfile;
uint16_t charge;

// Macro functions for opening battery and adapter files
#define OpenBattery() batfile.open(BatteryPath, std::ifstream::in);
#define OpenAdapter() adptfile.open(AdapterPath, std::ifstream::in);

/*
    Returns the battery charge in percent unless 
    disconnected or error, which returns TripPoint
*/
uint16_t BatteryCharge() noexcept
{
    if (batfile.good())
    {
        batfile.seekg(0);
        batfile >> charge;
    }
    else
    {
        OpenBattery();
        charge = TripPoint;
    }
    return charge;
}

// Returns true if adapter connected and no errors, false otherwise
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
        OpenAdapter();
        return false;
    }
}


// Program entry point
int main(void)
{
    // Open the battery and apdater files
    OpenBattery(); OpenAdapter();

    // While battery charge is good or we're connected to power
    while (BatteryCharge() > TripPoint || AdapterOnline())
        std::this_thread::sleep_for(std::chrono::seconds(charge*ChargeMultiplier));

    // Tell user we're going down
    #ifdef TriggerMessage
    std::cout << TriggerMessage << std::endl;
    system("wall " TriggerMessage);
    #endif

    // Spam the TriggerCommand until exit status is zero
    while (system(TriggerCommand))
        std::this_thread::sleep_for(std::chrono::seconds(charge));

    // System is shutting down, exit zero
    return 0;    
}
