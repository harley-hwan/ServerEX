#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

/*
- Ŭ���̾�Ʈ â���� ��/list�� �Է� �� ���� ���ӵ� Ŭ���̾�Ʈ�� IP�ּҿ� �г��� ���
- Ŭ���̾�Ʈ â���� ��/to [�г���] [�޽���]�� �Է� �� �ش� [�г���] ����ڿ��Ը� [�޽���]�� ������ ���
- �г��ӿ� �ش��ϴ� ����ڰ� ���ų� ������ �ش� ������ ���� ���� �̿� �ش��ϴ� �����޽��� ���
*/

#define BUF_SIZE 100
#define MAX_CLNT 256


unsigned HandleClnt(SOCKET hClntSock);
void SendMsg(char name[], char* msg, int len);
void ErrorHandling(const char* msg);



HANDLE hMutex;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	HANDLE  hThread;

	printf("Server\n");

	//if (argc != 2)
	//{
	//	printf("Usage : %s <port>\n", argv[0]); // ��Ʈ��ȣ Ȯ��
	//	exit(1);
	//}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr)); // �ʱ�ȭ
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9000);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

	printf("Connected client IP: %s \n", inet_ntoa(clntAdr.sin_addr));
	HandleClnt(hClntSock);
	
	
	closesocket(hServSock); // ���� ����
	WSACleanup(); // �ڿ� �Ҵ� ����
	return 0;
}

//127.0.0.1 : 9000     127.0.0.1 : 1234
//127.0.0.1 : 9000     127.0.0.1 : 1233


unsigned HandleClnt(SOCKET hClntSock)
{
	int strLen = 0, i;
	char msg[BUF_SIZE];
	char* pname;

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) > 0)
	{
		msg[strLen] = '\0';
		printf("received %s\n", msg);
		
		send(hClntSock, msg, strLen, 0);
		printf("send [%s]\n", msg);
	}

	closesocket(hClntSock);
	return 0;
}

void ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}