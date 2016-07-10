#pragma once

#include "EZSockets.h"
#include <time.h>

/*
		EZSERVER
TDP threaded server with basic functions & callbacks
Built-in timeout functionality
*/

struct ServInd {
	int server;
	int index;
};

class Server {
public:
	Server(std::string ip, int port, int max);//Port and max connections
	~Server();

	void SetPingLim(int timeout, int freq) { PING_TIMEOUT = timeout; PING_FREQ = freq; }//Set ping limits in milliseconds
	EZSocket& GetConnection(int index) { return connections[index]; }
	void SetConnection(int index, EZSocket &soc) {
		connections[index].SetSocket(*soc.GetSocket());
		connections[index].SetPartner(*soc.GetPartner());
	}
	EZSocket& GetSocket() { return soc_server; }

	void CallbackConnect(void(*f)(Server&, int)) { callback[0] = f; }//OnConnect
	void CallbackDisconnect(void(*f)(Server&, int)) { callback[1] = f; }//OnDisconnect
	void CallbackString(void(*f)(Server&, int, std::string)) { callback_str = f; }//OnReceiveString
	void CallbackInt(void(*f)(Server&, int, int)) { callback_int = f; }//OnReceiveInt

	void Send(int index, int val) { connections[index].SendInt(val); }
	void Send(int index, std::string val) { connections[index].SendString(val); }
	void Send(int index, char* val, int size) { connections[index].SendString(val, size); }

	void Disconnect(int index);

	void SendToAll(int val, int except = -1) {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (*(connections[i].GetSocket()) == 0 || i == except) { continue; }
			Send(i, val);
		}
	}
	void SendToAll(std::string val, int except = -1) {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (*(connections[i].GetSocket()) == 0 || i == except) { continue; }
			Send(i, val);
		}
	}
	void SendToAll(char* val, int length, int except = -1) {
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (*(connections[i].GetSocket()) == 0 || i == except) { continue; }
			Send(i, val, length);
		}
	}

	bool isRunning = false;
	int PING_TIMEOUT = 2500, PING_FREQ = 650;
	int MAX_CLIENTS = 0;
	unsigned int CONNECT_COUNT = 0;
	HANDLE* clientHandlers;
	HANDLE mainThread, clientThread;
	clock_t* pings, *pongs;
	ServInd* doubles;
	void(*callback[2])(Server&, int) = { 0,0 };//Connect, Disconnect, String, Int: Respectively
	void(*callback_str)(Server&, int, std::string) = 0;
	void(*callback_int)(Server&, int, int) = 0;

private:
	EZSocket* connections;
	EZSocket soc_server;
	bool deconstructed = false;
};