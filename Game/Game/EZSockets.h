#pragma once

#include <WS2tcpip.h>
//#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>

/*
			EZSOCKETS
Application of Windows TDP sockets
Quite messy alone | Organized into Client & Server
IMPORTANT :		InitWSA() & CleanupWSA()
*/

enum Packet {
	Packet_ERROR,
	Packet_PING,
	Packet_String,
	Packet_Int,
	Packet_DISCONNECT
};

void InitWSA();
HANDLE MakeThread(void(*f));
HANDLE MakeThread(void(*f)(int), int param);

void MergeInts(char* dest, Packet pack, int value);
void DivergeInts(char* src, Packet *pack, int *value);

class EZSocket {
public:
	EZSocket();
	~EZSocket();

	SOCKET* GetSocket() { return &mySoc; }
	SOCKADDR_IN* GetPartner() { return &partner; }
	void SetSocket(SOCKET soc) { mySoc = soc; }
	void SetPartner(SOCKADDR_IN part) { partner = part; }

	bool Connect(std::string ip, int port);
	void CloseConnection();//Also sends disconnect packet
	void Listen(std::string ip, int port);
	SOCKET Accept();

	bool SendString(const char* message, int length);
	bool SendString(std::string msg);
	bool SendInt(int num);
	bool SendPing();

	void ReceivePacket(Packet *pack, int *value);
	void ReceiveData(char* data, int length);
	std::string ReceiveString(int length);
private:
	SOCKET mySoc;
	SOCKADDR_IN partner;
};