#include "EZSockets.h"
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

HANDLE MakeThread(void(*f)){
	return CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)f, NULL, NULL, NULL);
}

HANDLE MakeThread(void(*f)(int), int param){
	return CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)f, (LPVOID)(param), NULL, NULL);
}

void MergeInts(char* dest, Packet pack, int value){
	memcpy(dest, &pack, sizeof(Packet));
	memcpy(dest + sizeof(Packet), &value, sizeof(int));
}

void DivergeInts(char* data, Packet *pack, int *value){
	memcpy(pack, data, sizeof(Packet));
	memcpy(value, data + sizeof(Packet), sizeof(int));
}

EZSocket::EZSocket(){
	mySoc = 0;
}

EZSocket::~EZSocket(){
	closesocket(mySoc);
	mySoc = 0;
}

void EZSocket::CloseConnection(){
	char data[sizeof(Packet) + sizeof(int)];
	MergeInts(data, Packet_DISCONNECT, 1);
	send(mySoc, data, sizeof(data), NULL);

	closesocket(mySoc);
	mySoc = 0;
}

bool EZSocket::Connect(std::string ip, int port){
	int partnerlen = sizeof(partner);
	partner.sin_family = AF_INET;
	//partner.sin_addr.s_addr = inet_addr(ip.c_str());
	InetPton(AF_INET, ip.c_str(), &partner.sin_addr.s_addr);
	partner.sin_port = htons(port);

	mySoc = socket(AF_INET, SOCK_STREAM, NULL);
	return (connect(mySoc, (SOCKADDR*)&partner, sizeof(partner)) == 0);
}

void EZSocket::Listen(std::string ip, int port){
	int partnerlen = sizeof(partner);
	partner.sin_family = AF_INET;
	//partner.sin_addr.s_addr = inet_addr(ip.c_str());
	InetPton(AF_INET, ip.c_str(), &partner.sin_addr.s_addr);
	partner.sin_port = htons(port);

	mySoc = socket(AF_INET, SOCK_STREAM, NULL);
	bind(mySoc, (SOCKADDR*)&partner, sizeof(partner));
	listen(mySoc, SOMAXCONN);
}

SOCKET EZSocket::Accept(){
	int partnerlen = sizeof(partner);
	return accept(mySoc, (SOCKADDR*)&partner, &partnerlen);
}

bool EZSocket::SendString(const char* message, int length){
	char data[sizeof(Packet)+sizeof(int)];
	MergeInts(data, Packet_String, length);
	if (send(mySoc, data, sizeof(data), NULL) == SOCKET_ERROR){
		//std::cout << WSAGetLastError() << std::endl;
		return false;
	}
	send(mySoc, message, length, NULL);
	return true;
}

bool EZSocket::SendString(std::string msg){
	return SendString(msg.c_str(), msg.size());
}

bool EZSocket::SendInt(int num){
	char data[sizeof(Packet)+sizeof(int)];
	MergeInts(data, Packet_Int, num);
	if (send(mySoc, data, sizeof(data), NULL) == SOCKET_ERROR){
		return false;
	}
	return true;
}

bool EZSocket::SendPing(){
	char data[sizeof(Packet)+sizeof(int)];
	MergeInts(data, Packet_PING, 1);
	return (send(mySoc, data, sizeof(data), NULL) != SOCKET_ERROR);
}

void EZSocket::ReceivePacket(Packet *pack, int *value){
	char data[sizeof(Packet)+sizeof(int)];
	if (recv(mySoc, data, sizeof(data), NULL) == SOCKET_ERROR){
		//std::cout << WSAGetLastError() << std::endl;
		*pack = Packet_ERROR;
		*value = 0;
	}
	else{
		DivergeInts(data, pack, value);
	}
}

void EZSocket::ReceiveData(char* data, int length){
	recv(mySoc, data, length, NULL);
}

std::string EZSocket::ReceiveString(int length){
	char* buffer = new char[length + 1];
	buffer[length] = '\0';
	ReceiveData(buffer, length);
	return std::string(buffer);
}

void InitWSA() {
	WORD sockV;
	WSADATA wsaData;
	sockV = MAKEWORD(2, 1);
	WSAStartup(sockV, &wsaData);
}