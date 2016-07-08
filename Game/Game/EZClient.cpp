#include "EZClient.h"

void ClientThread(Client* client) {
	Packet pack; int val;
	while (true) {
		client->host_soc.ReceivePacket(&pack, &val);
		if (pack == Packet_ERROR) {
			continue;
		}
		if (pack == Packet_String) {
			std::string temp = client->host_soc.ReceiveString(val);
			//STRING CALLBACK
			if (client->stringcallback != 0) {
				client->stringcallback(*client, temp);
			}
		}
		if (pack == Packet_PING) {
			//std::cout << "Ping received" << std::endl;
			client->pong = clock();
		}
		if (pack == Packet_Int) {
			//INT CALLBACK
			if (client->intcallback != 0) {
				client->intcallback(*client, val);
			}
		}
		if (pack == Packet_DISCONNECT) {
			TerminateThread(client->pingThread, 0);
			CloseHandle(client->pingThread);
			client->pingThread = INVALID_HANDLE_VALUE;

			CloseHandle(client->thread);
			client->thread = INVALID_HANDLE_VALUE;

			client->is_connected = false;

			//Disconnect callback
			if (client->discallback != 0) {
				client->discallback(*client, false);
			}
			break;
		}
	}
}

void ClientPing(Client* client) {
	while (true) {
		if (clock() >= client->ping + client->PING_FREQ) {//Time to send ping
			//std::cout << "Ping sent" << std::endl;
			client->SendPing();
			client->ping = clock();
		}
		if (clock() >= client->pong + client->PING_TIMEOUT) {//If timeout
			TerminateThread(client->thread, 0);
			CloseHandle(client->thread);
			client->thread = INVALID_HANDLE_VALUE;
			client->is_connected = false;
			break;
		}
	}
	CloseHandle(client->pingThread);
	client->pingThread = INVALID_HANDLE_VALUE;
	client->host_soc.CloseConnection();

	//TIMEOUT CALLBACK
	if (client->discallback != 0) {
		client->discallback(*client, true);
	}
}

Client::~Client() {
	Disconnect();
}

bool Client::Connect(std::string ip, int port) {
	host_ip = ip;
	host_port = port;
	is_connected = host_soc.Connect(host_ip, host_port);
	if (is_connected) {
		if (thread != INVALID_HANDLE_VALUE) {
			TerminateThread(thread, 0);
			CloseHandle(thread);
		}
		if (pingThread != INVALID_HANDLE_VALUE) {
			TerminateThread(pingThread, 0);
			CloseHandle(pingThread);
		}
		thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)(this), NULL, NULL);
		pingThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientPing, (LPVOID)(this), NULL, NULL);

		ping = clock();
		pong = clock();
		return true;
	}
	return false;
}

void Client::Disconnect() {
	host_soc.CloseConnection();

	is_connected = false;
	if (thread != INVALID_HANDLE_VALUE) {
		TerminateThread(thread, 0);
		CloseHandle(thread);
		thread = INVALID_HANDLE_VALUE;
	}
	if (pingThread != INVALID_HANDLE_VALUE) {
		TerminateThread(pingThread, 0);
		CloseHandle(pingThread);
		pingThread = INVALID_HANDLE_VALUE;
	}
}