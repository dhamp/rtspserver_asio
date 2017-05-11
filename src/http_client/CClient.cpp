#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "CClient.h"
#include "CHttpParser.h"
#include "CHttpSocket.h"

#define PATH2 "./http.html"

static char headbuf[MAX_LENGTH];

CClient::CClient()
{

}

CClient::~CClient()
{

}

int CClient::Start(char const* ip, char const* port ,char const* path)
{
	
	//socket ����������
	CHttpSocket chttpsock;
	
	sockfd= chttpsock.createSocket();

	if (-1 == sockfd)
	{
		printf("Create Socket Failed\n");
	}
	
	if (chttpsock.connectSocket(sockfd, ip, atoi(port)))
	{
		printf("Connect Success\r\n");
	}
	else
	{
		printf("Connenct Failed\n");
	}

//��ʽ��http  ͷ
	
	struct HTTP_TYPE httphead;
	
	httphead.http_method = M_GET;
	strcpy(httphead.http_url, ip); 
	strcpy(httphead.http_fpath,path);
	httphead.len=0;

	std::string clihead;
	CHttpParser chttpparser;
	
	clihead = chttpparser.formatHeader(httphead);
	
//����ͷ	
	int len = clihead.size();	
	chttpsock.sendData(sockfd, clihead.c_str(), len);

//������
	int headlen = 0;
	int bodylen = 0;
	//��ͷ
	memset(headbuf, 0, MAX_LENGTH);
	headlen = chttpsock.readHeader(sockfd, headbuf);
	//���� ͷ������HTTP  ��ĳ���
	bodylen = chttpparser.parser(headbuf, sizeof(headbuf));
	printf("bodylen:%d\n", bodylen);
	//��ȡHTTP������ļ�
	FILE* fp;
	char* bodybuf = new char[bodylen];
	
	fp = fopen(PATH2,"w+");	
	if(fp == NULL) 
	{
		perror("Open Failed:");
		delete bodybuf;
		return -1;
	}


	while(bodylen > 0)
	{
		int length = chttpsock.readData(sockfd, bodylen, bodybuf);
		//��������
		int ret = fwrite(bodybuf, length, 1, fp);
	
		if(ret < bodylen)
		{
		 	perror("Write File Compelete!\r\n");
		}

		bodylen -= length;
	}
		
	fclose(fp);
	delete bodybuf;

	return 0;
}

int CClient::Stop()
{
	close(sockfd);
	return 0;
}

