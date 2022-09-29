#include "RobotClass_LX16.h"

RobotClass_LX16 servo;

void setup() {
    servo.begin(&Serial);
    delay(1000);

    // old ID - 1
    // new ID - 2
    servo.setID(1, 2);
}

void loop() {
}