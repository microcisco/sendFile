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
#define SERVER_PORT 9211 //侦听端口  
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


	FILE * fp = fopen("C:\\Users\\baigu\\Pictures\\新建文件夹\\out.png", "rb+");
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //连接套接字  
	struct sockaddr_in saServer; //地址信息  

	BOOL fSuccess = TRUE;

	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	fseek(fp, 0, 0);

	//WinSock初始化  
	wVersionRequested = MAKEWORD(2, 2); //希望使用的WinSock DLL的版本  
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		system("pause");
		return 0;
	}
	//确认WinSock DLL支持版本2.2  
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		system("pause");
		return 0;
	}
	//创建Socket,使用TCP协议  
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		system("pause");
		return 0;
	}
	//构建服务器地址信息  
	saServer.sin_family = AF_INET; //地址家族  
	saServer.sin_port = htons(SERVER_PORT); //注意转化为网络节序  
	saServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.122");
	//连接服务器  
	std::cout << "开始连接" << std::endl;
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //关闭套接字  
		WSACleanup();
		system("pause");
		return 0;
	}

	std::cout << "连接成功" << std::endl;

	int nZero = 10;
	setsockopt(sClient, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));

	const char* g = "woshibaiguangan";
	//ret = send(sClient, g, 100, 0);

	/*
	std::ifstream input("H:\\镜像文件\\CentOS-7-x86_64-Minimal-1511_2.iso", std::ios::binary);
	input.get();

	long long totalBytes = 0;
	int curLength = 0;
	char buffer[10240] = { 0 };
	input.read(buffer , 10240);
	curLength = input.gcount();
	totalBytes += input.gcount();

	while (curLength != 0)
	{
		std::cout << "发送了" << (totalBytes / 1000) << "M";
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
	std::cout << "总大小" << size/(1024*1024)<<"M" << std::endl;

	std::ifstream in("F:\\WeGame\\data.zip", std::ios::binary);
	char buf[2048];
	long long totalBytes = 0;
	std::cout << "开始复制..." << std::endl;
	while (in)
	{
		in.read(buf, 2048);
		//gcount()用来提取读取的字节数，write将buf中的内容写入out流。
		send(sClient, buf, in.gcount(), 0);
		totalBytes += in.gcount();
		
		time_t it1 = time(NULL);
		if (it1 > it)
		{
			std::cout << "已经发送" << totalBytes / (1024 * 1024) << "M 还剩"<< (size - totalBytes)/(1024 * 1024)<<"M" << std::endl;
			it = it1;
		}
	}



	//while (end > 0)
	 while ( 0)
	{
		memset(data.sendMessage, '0', sizeof(data.sendMessage));
		fread(data.sendMessage, 1024, 1, fp);
		if (end >= 1024) //剩余很多信息  
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

	closesocket(sClient); //关闭套接字  
	WSACleanup();
	system("pause");
	return 0;
}