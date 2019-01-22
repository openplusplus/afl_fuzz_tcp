#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <fstream>

#define g_server_ip_addr "127.0.0.1"
#define MYPORT  7000
#define BUFFER_SIZE 1024*8

using namespace std;

int main(int argc,char **argv)
{

	string data;
	ifstream file;
	file.open(argv[1]);
	file>>data;

    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
 
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  //服务器端口
    servaddr.sin_addr.s_addr = inet_addr(g_server_ip_addr);  //服务器ip，inet_addr用于IPv4的IP转换（十进制转换为二进制）
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

	char *lpbuf = NULL;
	int   lpbuf_len=0;
	{
		char post[300],host[100],content_len[100];
		char* page = "/goform/formLogin";
		
		const char *header2="User-Agent: Tuobao Http 0.1\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept: */*\r\n";

		sprintf(post,"POST %s HTTP/1.1\r\n",page);
		sprintf(host,"HOST: %s:%d\r\n",g_server_ip_addr,MYPORT);
		sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(data.c_str()));

		lpbuf_len = strlen(post)+strlen(host)+strlen(header2)+strlen(content_len)+strlen(data.c_str())+1;
		lpbuf = (char*)malloc(lpbuf_len);
		if(lpbuf==NULL){
			return -1;
		}

		strcpy(lpbuf,post);
		strcat(lpbuf,host);
		strcat(lpbuf,header2);
		strcat(lpbuf,content_len);
		strcat(lpbuf,data.c_str());
	}
 
    char recvbuf[BUFFER_SIZE];

	send(sock_cli, lpbuf, lpbuf_len,0); ///发送
	recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
	fputs(recvbuf, stdout);

	//memset(sendbuf, 0, sizeof(sendbuf));//接受或者发送完毕后把数组中的数据全部清空（置0）
	//memset(recvbuf, 0, sizeof(recvbuf));
	sleep(1);

    close(sock_cli);
    return 0;
}