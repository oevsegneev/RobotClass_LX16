/*!
 *  @file RobotClass_LX16.h
 *  Servo LX16 class control library
 *  Written by Oleg Evsegneev for RobotClass (2022).
 *  e-mail: oleg.evsegneev@gmail.com
 */
 
#include <Arduino.h>
#include <Wire.h>

#define BAUD 115200

#define LX16_SERVO_FRAME_HEADER         0x55
#define LX16_SERVO_MOVE_TIME_WRITE      1
#define LX16_SERVO_MOVE_TIME_READ       2
#define LX16_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LX16_SERVO_MOVE_TIME_WAIT_READ  8
#define LX16_SERVO_MOVE_START           11
#define LX16_SERVO_MOVE_STOP            12
#define LX16_SERVO_ID_WRITE             13
#define LX16_SERVO_ID_READ              14
#define LX16_SERVO_ANGLE_OFFSET_ADJUST  17
#define LX16_SERVO_ANGLE_OFFSET_WRITE   18
#define LX16_SERVO_ANGLE_OFFSET_READ    19
#define LX16_SERVO_ANGLE_LIMIT_WRITE    20
#define LX16_SERVO_ANGLE_LIMIT_READ     21
#define LX16_SERVO_VIN_LIMIT_WRITE      22
#define LX16_SERVO_VIN_LIMIT_READ       23
#define LX16_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LX16_SERVO_TEMP_MAX_LIMIT_READ  25
#define LX16_SERVO_TEMP_READ            26
#define LX16_SERVO_VIN_READ             27
#define LX16_SERVO_POS_READ             28
#define LX16_SERVO_OR_MOTOR_MODE_WRITE  29
#define LX16_SERVO_OR_MOTOR_MODE_READ   30
#define LX16_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LX16_SERVO_LOAD_OR_UNLOAD_READ  32
#define LX16_SERVO_LED_CTRL_WRITE       33
#define LX16_SERVO_LED_CTRL_READ        34
#define LX16_SERVO_LED_ERROR_WRITE      35
#define LX16_SERVO_LED_ERROR_READ       36

class RobotClass_LX16 {
	HardwareSerial * _port = NULL;

	byte checkSum(byte buf[]);
	int receiveHandle(byte *ret);
public:
	RobotClass_LX16;
	void begin(HardwareSerial * port);
	void move(uint8_t id, int16_t position, uint16_t time);
	void stop(uint8_t id);
	void setID(uint8_t oldID, uint8_t newID);
	void setMode(uint8_t id, uint8_t Mode, int16_t Speed);
	void load(uint8_t id);
	void unload(uint8_t id);
	int readPosition(uint8_t id);
	int readVin(uint8_t id);
};