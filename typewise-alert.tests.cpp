#include <gtest/gtest.h>
#include "typewise-alert.h"

// Mocks
extern BreachType Mock_classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);
extern BreachType mock_breach;
extern BreachType (*Func_ptr_classifyTemperatureBreach)(CoolingType, double);

// Replace the actual classifyTemperatureBreach function with Mock_classifyTemperatureBreach
BreachType Mock_classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    return mock_breach;  // Return the pre-set breach type
}

// Test Cases
TEST(TypeWiseAlertTestSuite, TestCheckAndAlertLowBreachController) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = TOO_LOW;
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, -10);
    EXPECT_EQ(mock_breach, TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, TestCheckAndAlertHighBreachController) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = TOO_HIGH;
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, 50);
    EXPECT_EQ(mock_breach, TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, TestCheckAndAlertHighBreachEmail) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = TOO_HIGH;
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};
    checkAndAlert(TO_EMAIL, batteryChar, 50);
    EXPECT_EQ(mock_breach, TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, TestCheckAndAlertLowBreachEmail) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = TOO_LOW;
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};
    checkAndAlert(TO_EMAIL, batteryChar, -1);
    EXPECT_EQ(mock_breach, TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, TestCheckAndAlertNormalBreachController) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = NORMAL;
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, 5);
    EXPECT_EQ(mock_breach, NORMAL);
}

TEST(TypeWiseAlertTestSuite, TestCheckAndAlertBoundaryCondition) {
    Func_ptr_classifyTemperatureBreach = Mock_classifyTemperatureBreach;
    mock_breach = NORMAL;
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, 35);  // Upper boundary for PASSIVE_COOLING
    EXPECT_EQ(mock_breach, NORMAL);
}
