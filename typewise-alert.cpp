#include "typewise-alert.h"
#include <stdio.h>
#include <unordered_map>

typedef struct {
    double lowerLimit;
    double upperLimit;
} BreachLimits;

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachLimits getBreachLimits(CoolingType coolingType) {
    static const std::unordered_map<CoolingType, BreachLimits> limitsMap = {
        {PASSIVE_COOLING, {0, 35}},
        {HI_ACTIVE_COOLING, {0, 45}},
        {MED_ACTIVE_COOLING, {0, 40}}
    };

    auto it = limitsMap.find(coolingType);
    if (it != limitsMap.end()) {
        return it->second;
    } else {
        return {0, 0};  // Return a default limit if the coolingType is not found
    }
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    BreachLimits limits = getBreachLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

void sendAlert(AlertTarget alertTarget, BreachType breachType) {
    switch (alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    sendAlert(alertTarget, breachType);
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\nHi, the temperature is too low\n", recipient);
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\nHi, the temperature is too high\n", recipient);
    }
}
