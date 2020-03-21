#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>


class SerialPort
{

public:
	SerialPort(){}
	SerialPort(const char *portName)
	{
		this->connected = false;

		this->handler = CreateFileA(static_cast<LPCSTR>(portName),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (this->handler == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				printf("ERROR: Handle was not attached. Reason: %s not available\n", portName);
			}
			else
			{
				printf("ERROR!!!");
			}
		}
		else {
			DCB dcbSerialParameters = { 0 };

			if (!GetCommState(this->handler, &dcbSerialParameters)) {
				printf("failed to get current serial parameters");
			}
			else {
				dcbSerialParameters.BaudRate = CBR_9600;
				dcbSerialParameters.ByteSize = 8;
				dcbSerialParameters.StopBits = ONESTOPBIT;
				dcbSerialParameters.Parity = NOPARITY;
				dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

				if (!SetCommState(handler, &dcbSerialParameters))
				{
					printf("ALERT: could not set Serial port parameters\n");
				}
				else {
					this->connected = true;
					PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
					Sleep(ARDUINO_WAIT_TIME);
				}
			}
		}
	}

	~SerialPort()
	{
		if (this->connected) {
			this->connected = false;
			CloseHandle(this->handler);
		}
	}

	int readSerialPort(const char *buffer, unsigned int buf_size)
	{
		DWORD bytesRead;
		unsigned int toRead = 0;

		ClearCommError(this->handler, &this->errors, &this->status);

		if (this->status.cbInQue > 0) {
			if (this->status.cbInQue > buf_size) {
				toRead = buf_size;
			}
			else toRead = this->status.cbInQue;
		}

		if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

		return 0;
	}

	bool writeSerialPort(const char *buffer, unsigned int buf_size)
	{
		DWORD bytesSend;

		if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
			ClearCommError(this->handler, &this->errors, &this->status);
			return false;
		}
		else return true;
	}
	bool isConnected()
	{
		return this->connected;
	}

private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;
};


class stepMotor
{
public:
	stepMotor(std::string & port, std::string & cmd, std::string & signal) :
		m_portName { port },
		m_command  { cmd },
		m_signal { signal }
	{
		m_port = new SerialPort(m_portName.c_str());
	}

	bool isConnected()
	{
		return m_port->isConnected();
	}

	void oneStep()
	{
		while (m_port->isConnected())
		{
			m_port->writeSerialPort(m_command.c_str(), MAX_DATA_LENGTH)
		}
	}



private:

	SerialPort *m_port;

	std::string m_portName;
	std::string m_command;
	std::string m_signal;
};