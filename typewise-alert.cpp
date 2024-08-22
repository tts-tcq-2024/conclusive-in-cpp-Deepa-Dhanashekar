#include "typewise-alert.h"
#include <stdio.h>


BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

Limits set_the_limits(CoolingType coolingType) {
  Limits arr[] = {{0,35},{0,45},{0,40}};
  return arr[coolingType];
}

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
    Limits limits = set_the_limits(coolingType); 
    return inferBreach(temperatureInC, limits.lower_limit, limits.upper_limit);
}

// Function pointer
BreachType (*Func_ptr_classifyTemperatureBreach)(CoolingType, double) = classifyTemperatureBreach;

//Mock interface
BreachType mock_breach;
BreachType Mock_classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
    Limits limits = set_the_limits(coolingType); 
    mock_breach = inferBreach(temperatureInC, limits.lower_limit, limits.upper_limit);
    return mock_breach;
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = Func_ptr_classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if(breachType == TOO_LOW) {
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too low\n");
  }
  
  if(breachType == TOO_HIGH) {
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too high\n");
  }
}
