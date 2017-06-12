#pragma once
// https://blog.manash.me/serial-communication-with-an-arduino-using-c-on-windows-d08710186498

#define ARDUINO_WAIT_TIME 2000 
#define MAX_LENGTH 64  // max data length

#include <windows.h>
#include <iterator> 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "params.h"


class Serial {
private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;

public:
	Serial();
	~Serial();

	int read(char *buffer, unsigned int bufSize);
	bool write(char *buffer, unsigned int bufSize);

	void printLastError(char* name);
	bool isConnected();
};

