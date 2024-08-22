#include <gtest/gtest.h>
#include "typewise-alert.h"

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);
    EXPECT_EQ(inferBreach(15, 20, 30), TOO_LOW);
    EXPECT_EQ(inferBreach(35, 20, 30), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreach) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 30), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 40), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlert_High_Breach_Email) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandC"};
    checkAndAlert(TO_EMAIL, batteryChar, 36);  // Should send too high alert via email
}
 
TEST(TypeWiseAlertTestSuite, CheckAndAlert_Low_Breach_Email) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandD"};
    checkAndAlert(TO_EMAIL, batteryChar, 5);  // Should send too low alert via email
}
TEST(TypeWiseAlertTestSuite, CheckAndAlertController) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, 30);  // Should send normal alert to controller
}


TEST(TypeWiseAlertTestSuite, CheckAndAlertEmail) {
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};
    checkAndAlert(TO_EMAIL, batteryChar, 50);  // Should send too high alert via email
}
