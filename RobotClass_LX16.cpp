/*!
 *  @file RobotClass_LX16.cpp
 *  Servo LX16 class control library
 *  Written by Oleg Evsegneev for RobotClass (2022).
 *  e-mail: oleg.evsegneev@gmail.com
 */
#include "RobotClass_LX16.h"

#define GET_LOW_BYTE(A) (uint8_t)((A))
//Macro function  get lower 8 bits of A
#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)
//Macro function  get higher 8 bits of A
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))
//put A as higher 8 bits   B as lower 8 bits   which amalgamated into 16 bits integer

void RobotClass_LX16::begin(HardwareSerial * port){
	_port = port;
	_port->begin(BAUD);
}

byte RobotClass_LX16::checkSum(byte buf[]){
	byte i;
	uint16_t temp = 0;
	for (i = 2; i < buf[3] + 2; i++) {
		temp += buf[i];
	}
	temp = ~temp;
	i = (byte)temp;
	return i;
}

void RobotClass_LX16::move(uint8_t id, int16_t position, uint16_t time){
	byte buf[10];
	if(position < 0)
		position = 0;
	if(position > 1000)
		position = 1000;
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 7;
	buf[4] = LX16_SERVO_MOVE_TIME_WRITE;
	buf[5] = GET_LOW_BYTE(position);
	buf[6] = GET_HIGH_BYTE(position);
	buf[7] = GET_LOW_BYTE(time);
	buf[8] = GET_HIGH_BYTE(time);
	buf[9] = checkSum(buf);
	_port->write(buf, 10);
}

void RobotClass_LX16::stop(uint8_t id){
	byte buf[6];
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LX16_SERVO_MOVE_STOP;
	buf[5] = checkSum(buf);
	_port->write(buf, 6);
}

void RobotClass_LX16::setID(uint8_t oldID, uint8_t newID){
	byte buf[7];
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = oldID;
	buf[3] = 4;
	buf[4] = LX16_SERVO_ID_WRITE;
	buf[5] = newID;
	buf[6] = checkSum(buf);
	_port->write(buf, 7);
}

void RobotClass_LX16::setMode(uint8_t id, uint8_t Mode, int16_t Speed){
	byte buf[10];
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 7;
	buf[4] = LX16_SERVO_OR_MOTOR_MODE_WRITE;
	buf[5] = Mode;
	buf[6] = 0;
	buf[7] = GET_LOW_BYTE((uint16_t)Speed);
	buf[8] = GET_HIGH_BYTE((uint16_t)Speed);
	buf[9] = checkSum(buf);
	_port->write(buf, 10);
}
void RobotClass_LX16::load(uint8_t id){
	byte buf[7];
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 4;
	buf[4] = LX16_SERVO_LOAD_OR_UNLOAD_WRITE;
	buf[5] = 1;
	buf[6] = checkSum(buf);
	_port->write(buf, 7);
}

void RobotClass_LX16::unload(uint8_t id){
	byte buf[7];
	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 4;
	buf[4] = LX16_SERVO_LOAD_OR_UNLOAD_WRITE;
	buf[5] = 0;
	buf[6] = checkSum(buf);
	_port->write(buf, 7);
}


int RobotClass_LX16::receiveHandle(byte *ret){
	bool frameStarted = false;
	bool receiveFinished = false;
	byte frameCount = 0;
	byte dataCount = 0;
	byte dataLength = 2;
	byte rxBuf;
	byte recvBuf[32];
	byte i;

	while (_port->available()) {
		rxBuf = _port->read();
		delayMicroseconds(100);
		if (!frameStarted) {
			if (rxBuf == LX16_SERVO_FRAME_HEADER) {
				frameCount++;
				if (frameCount == 2) {
					frameCount = 0;
					frameStarted = true;
					dataCount = 1;
				}
			}
			else {
				frameStarted = false;
				dataCount = 0;
				frameCount = 0;
			}
		}
		if (frameStarted) {
			recvBuf[dataCount] = (uint8_t)rxBuf;
			if (dataCount == 3) {
				dataLength = recvBuf[dataCount];
				if (dataLength < 3 || dataCount > 7) {
					dataLength = 2;
					frameStarted = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 3) {
				if (checkSum(recvBuf) == recvBuf[dataCount - 1]) {
					frameStarted = false;
					memcpy(ret, recvBuf + 4, dataLength);
					return 1;
				}
				return -1;
			}
		}
	}
}
int RobotClass_LX16::readPosition(uint8_t id){
	int count = 10000;
	int ret;
	byte buf[6];

	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LX16_SERVO_POS_READ;
	buf[5] = checkSum(buf);

	while (_port->available())
		_port->read();
	
	_port->write(buf, 6);

	while (!_port->available()) {
		count -= 1;
		if (count < 0)
			return -2048;
	}

	if (receiveHandle(buf) > 0)
		ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
	else
		ret = -2048;
	return ret;
}

int RobotClass_LX16::readVin(uint8_t id){
	int count = 10000;
	int ret;
	byte buf[6];

	buf[0] = buf[1] = LX16_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LX16_SERVO_VIN_READ;
	buf[5] = checkSum(buf);

	while (_port->available())
		_port->read();

	_port->write(buf, 6);

	while (!_port->available()) {
		count -= 1;
		if (count < 0)
			return -2048;
	}

	if (receiveHandle(buf) > 0)
		ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
	else
		ret = -2049;

  return ret;
}
