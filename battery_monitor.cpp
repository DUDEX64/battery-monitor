#include <fstream>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <exception>

#include "battery_monitor.hpp"

std::ifstream batfile;
std::ifstream adptfile;
uint16_t charge = TripPoint;

// Macro functions for opening battery and adapter files
#define OpenBattery() batfile.open(BatteryPath, std::ifstream::in)
#define OpenAdapter() adptfile.open(AdapterPath, std::ifstream::in)

/*
    Returns the battery charge in percent unless 
    disconnected or error, which returns TripPoint
*/
uint16_t BatteryCharge() noexcept
{
    try 
    {
        batfile.seekg(0);
        batfile >> charge;
    }
    catch(...)
    {
        try {OpenBattery();} catch (...) {;};
        charge = TripPoint;
    }
    return charge;
}

// Returns true if adapter connected and no errors, false otherwise
bool AdapterOnline() noexcept
{
    try
    {
        bool online;
        adptfile.seekg(0);
        adptfile >> online;
        return online;
    }
    catch(...)
    {
        try {OpenAdapter();} catch(...) {;};
        return false;
    }
}


// Program entry point
int main(void)
{
    // Open the battery and apdater files
    OpenBattery(); OpenAdapter();

    // While battery charge is good or we're connected to power
    while (AdapterOnline() || BatteryCharge() > TripPoint)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<size_t>(static_cast<float>(charge)
                *static_cast<float>(ChargeMultiplier)
                *static_cast<float>(1000.0)
            )
        ));
    }

    // Close the battery and adapter files
    batfile.close(); adptfile.close();

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
