#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <fstream>  

#include <ratio>
#include <chrono>
#include <ctime>
#include <iostream>  
#include <stdio.h>  
#include <stdlib.h>  
#include <winsock2.h>  
#define SERVER_PORT 9211 //�����˿�  
#pragma comment(lib,"ws2_32.lib")  

struct Data
{
	int length;
	char sendMessage[2000];
	int fin;
}data;

int main()
{
	system("pause");


	FILE * fp = fopen("C:\\Users\\baigu\\Pictures\\�½��ļ���\\out.png", "rb+");
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //�����׽���  
	struct sockaddr_in saServer; //��ַ��Ϣ  

	BOOL fSuccess = TRUE;

	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	fseek(fp, 0, 0);

	//WinSock��ʼ��  
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾  
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		system("pause");
		return 0;
	}
	//ȷ��WinSock DLL֧�ְ汾2.2  
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		system("pause");
		return 0;
	}
	//����Socket,ʹ��TCPЭ��  
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		system("pause");
		return 0;
	}
	//������������ַ��Ϣ  
	saServer.sin_family = AF_INET; //��ַ����  
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������  
	saServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.122");
	//���ӷ�����  
	std::cout << "��ʼ����" << std::endl;
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���  
		WSACleanup();
		system("pause");
		return 0;
	}

	std::cout << "���ӳɹ�" << std::endl;

	int nZero = 10;
	setsockopt(sClient, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));

	const char* g = "woshibaiguangan";
	//ret = send(sClient, g, 100, 0);

	/*
	std::ifstream input("H:\\�����ļ�\\CentOS-7-x86_64-Minimal-1511_2.iso", std::ios::binary);
	input.get();

	long long totalBytes = 0;
	int curLength = 0;
	char buffer[10240] = { 0 };
	input.read(buffer , 10240);
	curLength = input.gcount();
	totalBytes += input.gcount();

	while (curLength != 0)
	{
		std::cout << "������" << (totalBytes / 1000) << "M";
		ret = send(sClient, buffer, curLength, 0);

		input.read(buffer, 10240);
		curLength = input.gcount();
		totalBytes += input.gcount();
	}
	*/

	static time_t it = time(NULL);


	HANDLE fhadle = CreateFile("F:\\WeGame\\data.zip", 0, 0, 0, OPEN_EXISTING, 0, 0);
	//HANDLE fhadle = CreateFile("file.txt", 0, 0, 0, OPEN_EXISTING, 0, 0);
	DWORD size = GetFileSize(fhadle, 0);
	std::cout << "�ܴ�С" << size/(1024*1024)<<"M" << std::endl;

	std::ifstream in("F:\\WeGame\\data.zip", std::ios::binary);
	char buf[2048];
	long long totalBytes = 0;
	std::cout << "��ʼ����..." << std::endl;
	while (in)
	{
		in.read(buf, 2048);
		//gcount()������ȡ��ȡ���ֽ�����write��buf�е�����д��out����
		send(sClient, buf, in.gcount(), 0);
		totalBytes += in.gcount();
		
		time_t it1 = time(NULL);
		if (it1 > it)
		{
			std::cout << "�Ѿ�����" << totalBytes / (1024 * 1024) << "M ��ʣ"<< (size - totalBytes)/(1024 * 1024)<<"M" << std::endl;
			it = it1;
		}
	}



	//while (end > 0)
	 while ( 0)
	{
		memset(data.sendMessage, '0', sizeof(data.sendMessage));
		fread(data.sendMessage, 1024, 1, fp);
		if (end >= 1024) //ʣ��ܶ���Ϣ  
		{
			data.fin = 0;
			data.length = 1024;
		}
		else
		{
			data.fin = 1;
			data.length = end;
		}
		//ret = send(sClient, (char *)&data, sizeof(struct Data), 0);
		const char* g = "woshibaiguangan";
		ret = send(sClient, g, sizeof(g), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send() failed!\n");
		}
		else
		{
			end -= 1024;
		}
	}

	closesocket(sClient); //�ر��׽���  
	WSACleanup();
	system("pause");
	return 0;
}