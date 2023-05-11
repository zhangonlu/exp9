/***************************************************************
*            文件：      communication.h
*            描述：      上线、聊天、传送文件功能头文件
****************************************************************/
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include "myinclude.h"
#define PORT 2425
//接收消息线程，接收其他客户端发送的UDP数据
void *recv_msg_thread(void *arg);
//发送文件线程，等待其他客户端接收文件并负责向其传送文件
void *sendfile_thread(void *arg);
void online(char *user, char *host); //上线
void ipmsg_exit(void); //下线
//发送信息
void msg_send(char *msg, int len, struct sockaddr_in addr);
//接收文件（参数为接收文件列表中的序号）
int recvfile(int id);
char *user(void); 	//获取用户名
char *host(void);	//获取主机名
int udp_fd(void); 	//获取UDP描述符
#endif	//_COMMUNICATION_H_