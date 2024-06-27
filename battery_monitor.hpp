// The path to the battery's remaining capactiy file
#define BatteryPath "/sys/class/power_supply/BAT0/capacity"

// The path to the adapter's online file
#define AdapterPath "/sys/class/power_supply/ACAD/online"

// The battery percentage to at which we should shutdown the system
#define TripPoint 30

// Multiply the current battery charge by this number to get the polling interval in seconds
#define ChargeMultiplier 5

// The command to run when we want to power-down the system
#define TriggerCommand "poweroff"

/* The message to display before calling TriggerCommand
    OPTIONAL: Comment if no message desired. */
#define TriggerMessage "System shutdown due to power supply!"