#include "serial.h"


Serial::Serial() {
	connected = false;
	DCB dcbParams;
	COMMTIMEOUTS timeouts = {0, 0, 0, 0, 0};

	handler = CreateFile(
		ARDUINO_PORT,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	
	if (!SetCommTimeouts(handler, &timeouts)) printLastError("Setting timeouts failed");

	if (handler == INVALID_HANDLE_VALUE) printLastError("Error opening file");

	
	if (!GetCommState(handler, &dcbParams)) {
		printLastError("Failed to get current serial parameters");

	} else {
		dcbParams.BaudRate = CBR_128000;
		dcbParams.ByteSize = 8;
		dcbParams.StopBits = ONESTOPBIT;
		dcbParams.Parity = NOPARITY;
		dcbParams.fDtrControl = DTR_CONTROL_DISABLE;
		dcbParams.fRtsControl = RTS_CONTROL_DISABLE;

		if (!SetCommState(handler, &dcbParams)) {
			printLastError("Could not set serial port parameters");

		} else {
			connected = true;
			PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
			Sleep(ARDUINO_WAIT_TIME);
		}
	}
}


Serial::~Serial() {
	if (connected) {
		connected = false;
		CloseHandle(handler);
	}
}


int Serial::read(char* buffer, unsigned int bufSize) {
	DWORD bytesRead;
	unsigned int toRead;

	ClearCommError(handler, &errors, &status);

	if (status.cbInQue > 0) {
		if (status.cbInQue > bufSize) toRead = bufSize;
		else toRead = status.cbInQue;
	}

	if (ReadFile(handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

	return 0;
}


bool Serial::write(char* buffer, unsigned int bufSize) {
	DWORD bytesSend;

	if (!WriteFile(handler, (void*)buffer, bufSize, &bytesSend, 0)) {
		ClearCommError(handler, &errors, &status);
		return false;

	} else return true;
}


void Serial::printLastError(char* name) {
	char *ptr = NULL;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		GetLastError(),
		0,
		(char *)&ptr,
		1024,
		NULL
	);

	fprintf(stderr, "\n%s: %s\n", name, ptr);
	LocalFree(ptr);
}


bool Serial::isConnected() {
	return connected;;
}