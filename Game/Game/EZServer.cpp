#include "EZServer.h"

static void ClientHandler(ServInd* dubparam) {
	Server* server = (Server*)dubparam->server;
	int index = dubparam->index;
	int value;
	Packet packtype;
	while (true) {
		server->GetConnection(index).ReceivePacket(&packtype, &value);
		if (packtype == Packet_ERROR) {
			continue;
		}
		if (packtype == Packet_PING) {
			server->pings[index] = clock();
			//std::cout << "Ping received" << std::endl;
		}
		if (packtype == Packet_String) {
			std::string temp = server->GetConnection(index).ReceiveString(value);
			//OnReceiveString()
			if (server->callback_str != 0) {
				server->callback_str(*server, index, temp);
			}
		}
		if (packtype == Packet_Int) {
			//OnReceiveInt()
			if (server->callback_int != 0) {
				server->callback_int(*server, index, value);
			}
		}
	}
}

static void ServerRun(Server *server) {
	EZSocket newConn, mainsoc = server->GetSocket();
	while (true) {
		newConn.SetSocket(mainsoc.Accept());

		if (*(newConn.GetSocket()) == 0) {
			std::cout << "Connection failed" << std::endl;
			newConn.CloseConnection();
			continue;
		}
		int index = -1;
		for (int i = 0; i < server->MAX_CLIENTS; i++) {
			if (*((server->GetConnection(i)).GetSocket()) == 0) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "Connections full: a connection has been denied." << std::endl;
			newConn.CloseConnection();
			continue;
		}
		server->pings[index] = clock();
		server->pongs[index] = clock();

		server->SetConnection(index, newConn);
		server->CONNECT_COUNT++;

		server->clientHandlers[index] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(&server->doubles[index]), NULL, NULL);

		//OnConnect()
		if (server->callback[0] != 0) {
			server->callback[0](*server, index);
		}
	}
}

static void ClientPinger(Server *server) {
	while (true) {
		for (int i = 0; i < server->MAX_CLIENTS; i++) {
			if (*(server->GetConnection(i).GetSocket()) == 0) { continue; }
			if (clock() >= server->pongs[i] + server->PING_FREQ) {//Time to send pong
				server->GetConnection(i).SendPing();
				server->pongs[i] = clock();
				//std::cout << "Pong sent" << std::endl;
			}
			if (clock() >= server->pings[i] + server->PING_TIMEOUT) {//If timeout
				server->CONNECT_COUNT--;
				server->GetConnection(i).CloseConnection();
				if (server->isRunning) { TerminateThread(server->clientHandlers[i], 0); CloseHandle(server->clientHandlers[i]); }
				//OnDisconnect()
				if (server->callback[1] != 0) {
					server->callback[1](*server, i);
				}
			}
		}
	}
}

void Server::Disconnect(int index) {
	if (*(connections[index].GetSocket()) == 0) { return; }
	TerminateThread(clientHandlers[index], 0);
	CloseHandle(clientHandlers[index]);
	clientHandlers[index] = INVALID_HANDLE_VALUE;
	connections[index].CloseConnection();
}

Server::Server(std::string ip, int port, int max) {
	callback[0] = 0; callback[1] = 0;
	callback_str = 0; callback_int = 0;
	soc_server.Listen(ip, port);
	MAX_CLIENTS = max;

	connections = new EZSocket[max];
	pings = new clock_t[max];
	pongs = new clock_t[max];
	clientHandlers = new HANDLE[max];
	doubles = new ServInd[max];
	for (int i = 0; i < max; i++) {
		doubles[i].index = i;
		doubles[i].server = (int)this;
	}

	mainThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerRun, (LPVOID)(this), NULL, NULL);
	clientThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientPinger, (LPVOID)(this), NULL, NULL);

	isRunning = true;
}

Server::~Server() {
	if (deconstructed) { return; }
	TerminateThread(clientThread,0);
	CloseHandle(clientThread);
	TerminateThread(mainThread,0);
	CloseHandle(mainThread);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (*(connections[i].GetSocket()) == 0){ continue; }
		TerminateThread(clientHandlers[i], 0);
		CloseHandle(clientHandlers[i]);
		clientHandlers[i] = INVALID_HANDLE_VALUE;
		connections[i].CloseConnection();
	}

	delete[] connections; delete[] clientHandlers;
	delete[] pings; delete[] pongs;
	delete[] doubles;

	soc_server.CloseConnection();
	deconstructed = true;
}