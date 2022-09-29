#include "RobotClass_LX16.h"

RobotClass_LX16 servo;
byte id = 1;

void setup() {
	servo.begin(&Serial);
	delay(1000);
}

void loop() {
	servo.move(id, 100, 500);
	delay(1000);
	servo.move(id, 500, 500);
	delay(1000);
	servo.move(id, 900, 500);
	delay(1000);
	servo.move(id, 500, 500);
	delay(1000);
}