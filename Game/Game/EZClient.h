#pragma once

#include "EZSockets.h"
#include <time.h>
#include <iostream>

/*
		EZCLIENT
TDP client with basic socket functions
Built-in timeout functionality
*/

class Client {
public:
	Client() {}
	~Client();

	void Send(std::string msg) { host_soc.SendString(msg); }
	void Send(char* msg, int length) { host_soc.SendString(msg, length); }
	void Send(int value) { host_soc.SendInt(value); }
	void SendPing() { host_soc.SendPing(); }

	void CallbackDisconnect(void(*f)(Client&, bool)) { discallback = f; }//OnDisconnect
	void CallbackInteger(void(*f)(Client&, int)) { intcallback = f; }//OnInteger
	void CallbackString(void(*f)(Client&, std::string)) { stringcallback = f; }//OnString

	bool Connect(std::string ip, int port);
	void Disconnect();

	clock_t ping, pong;
	HANDLE thread = INVALID_HANDLE_VALUE, pingThread = INVALID_HANDLE_VALUE;
	bool is_connected;
	int PING_FREQ = 650, PING_TIMEOUT = 2500;
	EZSocket host_soc;

	void(*discallback)(Client&, bool) = 0;
	void(*intcallback)(Client&, int) = 0;
	void(*stringcallback)(Client&, std::string) = 0;
private:
	std::string host_ip; int host_port;
};