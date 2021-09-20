#include "stdafx.h"
#include "Winsock2.h"
#include "iostream"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

SOCKET GetListenSocket(short shPortNo, int nBacklog = SOMAXCONN) {
	SOCKET hsoListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hsoListen == INVALID_SOCKET)
	{
		cout << "socket failed, code : " << WSAGetLastError() << endl;
		return INVALID_SOCKET;
	}

	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(SOCKADDR_IN));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(shPortNo);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	LONG lSockRet = bind(hsoListen, (PSOCKADDR)&sa, sizeof(SOCKADDR_IN));
	if (lSockRet == SOCKET_ERROR)
	{
		cout << "bind failed, code : " << WSAGetLastError() << endl;
		closesocket(hsoListen);
		return INVALID_SOCKET;
	}

	lSockRet = listen(hsoListen, nBacklog);
	if (lSockRet == SOCKET_ERROR)
	{
		cout << "listen failed, code : " << WSAGetLastError() << endl;
		closesocket(hsoListen);
		return INVALID_SOCKET;
	}

	return hsoListen;
}

SOCKET g_sockMain = INVALID_SOCKET;
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	if (g_sockMain != INVALID_SOCKET)
		closesocket(g_sockMain);
	return TRUE;
}

#define MAX_CLI_CNT 10

void _tmain() {
	WSADATA wsd;
	int nErrCode = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (nErrCode)
	{
		cout << "WSAStartup failed with error : " << nErrCode << endl;
		return;
	}

	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
	{
		cout << "SetConsoleCtrlHandler failed, code : " << GetLastError() << endl;
		return;
	}

	SOCKET hsoListen = GetListenSocket(9001, MAX_CLI_CNT);
	if (hsoListen == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	g_sockMain = hsoListen;
	cout << " ==> Waiting for client's connection......" << endl;

	fd_set fdr;
	SOCKET asoChilds[MAX_CLI_CNT];
	memset(asoChilds, 0xFF, MAX_CLI_CNT * sizeof(SOCKET));

	// 1. FD_ZERO를 이용해 사용하고자 하는 fd_set을 초기화
	// 2. FD_SET을 이용해 상태 변경을 감시하고자 하는 소켓을 추가
	// 3. select 함수를 호출해 상태 변경을 기다린다. 상태 변경이 있으면 fd_set은 갱신되고 select 함수는 리턴된다.
	// 타임아웃 값을 지정해 그 시간 동안 상태 변경이 없으면 select 함수는 0을 리턴하고 코드는 1의 과정으로 돌아간다.
	// 4. select 함수의 호출 결과가 0보다 크면 FD_ISSET 매크로를 이용해 변경이 발생한 소켓을 체크
	// 5. 변경이 발생한 소켓의 변경 내용에 따라 accept, read, write, closesocket 함수 등을 작업하고 1의 과정으로 돌아간다.

	while (true) {
		FD_ZERO(&fdr);
		FD_SET(hsoListen, &fdr);
		for (int i = 0; i < MAX_CLI_CNT; i++) {
			if (asoChilds[i] != INVALID_SOCKET)
				FD_SET(asoChilds[i], &fdr);
		}

		int nAvail = select(0, &fdr, NULL, NULL, NULL);
		if (nAvail == SOCKET_ERROR)
		{
			cout << "WSAStartup failed with error : " << nErrCode << endl;
			break;
		}
		if (nAvail == 0)
			continue;

		if (FD_ISSET(hsoListen, &fdr)) {
			SOCKET sock = accept(hsoListen, NULL, NULL);
			if (sock == INVALID_SOCKET) {
				LONG lErrCode = WSAGetLastError();
				if (lErrCode == WSAEINTR || lErrCode == WSAENOTSOCK)
					hsoListen = INVALID_SOCKET;
				else
					cout << "accept failed, code : " << lErrCode << endl;
				break;
			}

			for (int i = 0; i < MAX_CLI_CNT; i++) {
				if (asoChilds[i] == INVALID_SOCKET) {
					asoChilds[i] = sock;
					cout << " ==> New client " << sock << " connected..." << endl;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < MAX_CLI_CNT; i++) {
				SOCKET sock = asoChilds[i];
				if (sock == INVALID_SOCKET)
					continue;

				if (!FD_ISSET(sock, &fdr))
					continue;

				char szBuff[512];
				LONG lSockRet = recv(sock, szBuff, sizeof(szBuff), 0);
				if (lSockRet == SOCKET_ERROR)
				{
					//WSAECONNRESET
					cout << "recv failed, code : " << WSAGetLastError() << endl;
					closesocket(sock);
					asoChilds[i] = INVALID_SOCKET;
					continue;
				}
				if (lSockRet == 0)
				{
					closesocket(sock);
					asoChilds[i] = INVALID_SOCKET;
					cout << " ==> Client " << sock << " disconnected..." << endl;
				}
				else
				{
					szBuff[lSockRet] = 0;
					cout << " *** Client(" << sock << ") sent : " << szBuff << endl;

					lSockRet = send(sock, szBuff, lSockRet, 0);
					if (lSockRet == SOCKET_ERROR)
						cout << "send failed, code : " << WSAGetLastError() << endl;
				}
			}
		}
	}

	if (hsoListen != INVALID_SOCKET)
		closesocket(hsoListen);
	for (int i = 0; i < MAX_CLI_CNT; i++)
	{
		if (asoChilds[i] != INVALID_SOCKET)
			closesocket(asoChilds[i]);
	}

	WSACleanup();
}