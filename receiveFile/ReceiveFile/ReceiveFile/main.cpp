#define _CRT_SECURE_NO_WARNINGS

#include <iostream>  
#include <fstream>  
#include <stdio.h>  
#include <winsock2.h>  
#include <winsock2.h>  
#define SERVER_PORT 9211 //�����˿�  
#pragma comment(lib,"ws2_32.lib")  

struct Data //���ݰ�  
{
	int length;
	char receivemessage[2000]; //������Ϣ  
	int fin;
}data;

int totalLength = 10240;
int curLength = 0;
char buffer[10240] = {0};

int main()
{
	
	WORD wVersionRequested;//˫�ֽڣ����ֽڴ�ȡ���汾�ţ����ֽڴ�ȡ�߰汾�š�  
	WSADATA wsaData;//��ȡ�׽���ʵ�ֵľ�����Ϣ  
	int ret, length;
	SOCKET sListen, sServer; //�����׽��֣������׽���  
	struct sockaddr_in saServer, saClient; //��ַ��Ϣ    

	//WinSock��ʼ��  
	wVersionRequested = MAKEWORD(2, 2);
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return 0;
	}
	//����Socket,ʹ��TCPЭ��  
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() faild!\n");
		return 0;
	}
	//�������ص�ַ��Ϣ  
	saServer.sin_family = AF_INET; //��ַ����  
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�����ֽ���  
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ   

													   //��  
	ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���  
		WSACleanup();
		return 0;
	}

	//������������  
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���  
		return 0;
	}

	printf("Waiting for client connecting!\n");
	printf("Tips: Ctrl+c to quit!\n");
	length = sizeof(saClient);
	sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
	if (sServer == INVALID_SOCKET)
	{
		printf("accept() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���  
		WSACleanup();
		return 0;
	}


	std::ofstream output("d:\\out", std::ios::binary);
	while (true)
	{
		ret = recv(sServer, buffer, totalLength, 0);  //�ڶ�������ʹ��ǿ�����ͣ�Ϊһ�����ݰ�  
		if (ret == 0) //�ͻ����Ѿ��ر�����  
		{
			output.close();
			break;
			
		}
		else
		{
			output.write(buffer, ret);
		}
	
	}

	std::cout << "����ok";
	system("pause");



	//�����ȴ����ܿͻ�������  
	/*
	while (!data.fin)
	{
		memset(data.receivemessage, '0', sizeof(data.receivemessage));
		ret = recv(sServer, (char *)&data, sizeof(struct Data), 0);  //�ڶ�������ʹ��ǿ�����ͣ�Ϊһ�����ݰ�  
		if (ret == SOCKET_ERROR)
		{
			printf("recv() failed!\n");
			return 0;
		}
		if (ret == 0) //�ͻ����Ѿ��ر�����  
		{
			printf("Client has closed the connection\n");
			break;
		}
		fwrite(data.receivemessage, data.length, 1, fp);
	}
	*/

	closesocket(sListen);
	closesocket(sServer);
	WSACleanup();
	

	return 0;
}