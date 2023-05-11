/***************************************************************
*            文件：      file_manager.h
*            描述：      发送文件头文件
****************************************************************/
#ifndef _FILE_H_
#define _FILE_H_
#define SENDFILE	0
#define RECVFILE	1
typedef struct filelist{		//文件信息结构体
	    char name[50];
	    int num;
	    long pkgnum;
	    long size;
	    long ltime;
	    char user[10];	
	    struct filelist *next;
}IPMSG_FILE;
//向链表（接收或发送链表）中添加一个文件，如
//flag : SENDFILE（发送链表）或RECVFILE（接收链表）
void add_file(IPMSG_FILE temp, int flag);
//从链表（接收或发送链表）中删除一个文件
//flag : SENDFILE（发送链表）或RECVFILE（接收链表）
void del_file(IPMSG_FILE *temp, int flag);
//在接收链表中按照序号（id）查找文件
IPMSG_FILE * find_file(int id);
//根据包标号和文件序号从发送链表中获取文件
IPMSG_FILE *getfileinfo(long pkgnum, int filenum);
//获取文件属性并保存到fileopt中，将文件信息保存到发送链表中
int getfileopt(char *fileopt, char *filename, long pkgnum, int num);
//打印接收文件链表
void file_list(void);
//void free_link(void);
#endif	//_FILE_H_
/*****************************************************************            文件：      communication.c
*            描述：      上线、聊天、传送文件
****************************************************************/
#include  "myinclude.h"
#include  "communication.h"
#include  "user_manager.h"
#include  "file_manager.h"
static  int  udpfd  =  0;
static  int  tcpfd  =  0;
static  char  user_name[20]  =  "";
static  char  host_name[30]  =  "";
/****************************************************************
函	数：	void  create_server(void)
功	能：	创建UDP  和  TCP_Server  套接口
参	数：	无
****************************************************************/
void  create_server(void){
	    int  broadcast=1;
	    struct  sockaddr_in  addr  =  {AF_INET};
	    addr.sin_port  =  htons(PORT);
	    addr.sin_addr.s_addr  =  htonl(INADDR_ANY);
	    //Create  TCP  socket      for  SendFile  Server
	    tcpfd  =  socket(AF_INET,SOCK_STREAM,0);
	    if(tcpfd  <  0) {
	        perror("Socket  TCP");
	        exit(-1);
	    }
	    if(bind(tcpfd,  (struct  sockaddr*)&addr,  sizeof(addr))<0) {
	        perror("Bind  UDP");
	        exit(1);
	    }
	    listen(tcpfd,  10);
	    //Create  UDP  socket  for  commucation
	    udpfd  =  socket(AF_INET,SOCK_DGRAM,0);
	    if(udpfd  <  0) {
	        perror("Socket  UDP");
	        exit(-1);
	    }
	    if(bind(udpfd,  (struct  sockaddr*)&addr,  sizeof(addr))<0) {
	       		 perror("Bind  UDP");
	        		exit(1);
	     }	    setsockopt(udpfd,SOL_SOCKET,SO_BROADCAST,&broadcast,  sizeof(int));
}
/******************************************************************************************************************
函	数：void  broad_cast_online_info(void)
功	能：上线广播
参	数：无
******************************************************************************************************************/
void  broad_cast_online_info(void){
	    char  buf[100]="";
	    struct  sockaddr_in  addr  =  {AF_INET};
	    int  t  =  time((time_t  *)NULL);
	    int  len  =  sprintf(buf,"1:%d:%s:%s:%ld:%s", \t,user_name,host_name,IPMSG_BR_ENTRY,user_name);
	    addr.sin_port  =  htons(PORT);
	    //    addr.sin_addr.s_addr=inet_addr("255.255.255.255");	
	    addr.sin_addr.s_addr  =  htonl(-1);			//modified  by  wangyanjun  in  10/7/9
	    sendto(udpfd,  buf,  len,  0,  (struct  sockaddr*)&addr,sizeof(addr));	
}
/*******************************************
函	数：int  tcp_fd(void)
功	能：获取TCP套接字
参	数：无
*******************************************/
int  tcp_fd(void){   return  tcpfd; }
/****************************************************************
函	数：int  udp_fd(void)
功	能：获取UDP套接字
参	数：无
****************************************************************/
int  udp_fd(void){    return  udpfd; }
/****************************************************************
函	数：char  *user(void)
功	能：获取用户名称
参	数：无
****************************************************************/
char  *user(void)
{
	    return  user_name;
}

/****************************************************************
函	数：char  *host(void)
功	能：获取主机名
参	数：无****************************************************************/
char  *host(void)
{
	    return  host_name;
}
/****************************************************************
函	数：void  IPMsg_exit(void)
功	能：下线广播
参	数：无
****************************************************************/
void  IPMsg_exit(void)
{
	    char  buf[100]="";
	    struct  sockaddr_in  addr  =  {AF_INET};
	    int  t  =  time((time_t  *)NULL);
	    int  len  =  sprintf(buf,"1:%d:%s:%s:%ld:%s", 
 \t,user_name,host_name,IPMSG_BR_EXIT,user_name);
	    addr.sin_port  =  htons(PORT);
	    addr.sin_addr.s_addr=inet_addr("255.255.255.255");		
	    sendto(udpfd,  buf,  len,  0,  (struct  sockaddr*)
			&addr,sizeof(addr));	
}
/****************************************************************
函	数：void  online(char  *user,  char  *host)
功	能：上线并初始化系统
参	数：char  *user,  char  *host
****************************************************************/
void  online(char  *user,  char  *host)
{
	    strcpy(user_name,user);
	    strcpy(host_name,host);
	    create_server();
	    broad_cast_online_info();
}
/***************************************************************
函	数：void  msg_send(char  *msg,  int  len, struct  sockaddr_in  addr)
功	能：发送消息
参	数：char  *msg,  int  len, struct  sockaddr_in  addr
****************************************************************/
void  msg_send(char  *msg,  int  len, struct  sockaddr_in  addr)
{
	sendto(udpfd,  msg,  len,  0,  (struct  sockaddr*)&addr,  	sizeof(addr));
}
/****************************************************************
函	数：int  recvfile(int  id)
功	能：接收文件（参数为接收文件列表中的序号）
参	数：int  id
****************************************************************/
int  recvfile(int  id){
	    int  fd  =  0;
	    char  buf[2048]="";
	    FILE  *fp  =  NULL;
	    unsigned  long  len  =  0;
	    struct  sockaddr_in  addr  =  {AF_INET};
	    int  s_addr  =  0;
	    IPMSG_FILE  *p  =  find_file(id);	 //是否存在该文件
	    if(  p==NULL	)  {
	        IPMSG_OUT_MSG_COLOR( printf("no  such  file  id\n");  )
	        return  -1;   }
	    //根据发送者姓名获取发送者地址
	    s_addr  =  get_addr_by_name(p->user);	
	    if(  s_addr  ==  0  )	    {
	        IPMSG_OUT_MSG_COLOR(
	        printf("recv  file  error:  user  is  not  online!\n");     )
	        del_file(p,  RECVFILE);
	        return  -1;
	    }
	    fd  =  socket(AF_INET,  SOCK_STREAM,  0);	//创建临时TCP Client，用于接收文件
	    if(  fd  <  0  )  {
	        IPMSG_OUT_MSG_COLOR(printf("recv  file  error:  creat  socket  error!\n"); )
	        return  -1;
	    }
	    addr.sin_port  =  htons(PORT);
	    addr.sin_addr.s_addr  =  s_addr;
	    if(connect(fd,  (struct  sockaddr*)&addr,  sizeof(addr))!=0){
	        perror("recvfile  connect");
	        return  -1;
	    }
	    len  =  sprintf(buf,  "1:%ld:%s:%s:%ld:%lx:%d:0",  
				time((time_t*)NULL),user(),  host(), 
				IPMSG_GETFILEDATA,  p->pkgnum,  p->num);
	    send(fd,  buf,  len,  0);		//发送IPMSG_GETFILEDATA
	    fp  =  fopen(p->name,  "w");
	    if(  fp==NULL  ) {
	        perror("savefile");
	        return  -1;
	    }
	    len  =  0;	
	    do	 { 			//接收文件
		int  rlen  =  recv(fd,  buf,  sizeof(buf),  0);
	         len  +=  rlen;
	         IPMSG_OUT_MSG_COLOR(printf("\rrecvlen=%d%%",
							 (int)((100*len)/p->size)); )
	         fflush(stdout);
	         fwrite(buf,  1,  rlen,  fp);
	    }while(len  <  p->size);
	    printf("\n");
	    close(fd);	//关闭TCP  Client
	    fclose(fp);  //关闭文件
	    del_file(p,  RECVFILE);	//从文件列表中删除接收过的文件
	    return  0;
}
/************************************************************
函	数：void  *sendfile_thread(void  *arg)
功	能：发送文件的线程
参	数：void  *arg
************************************************************/
void  *sendfile_thread(void  *arg){
	    int  fd  =  tcp_fd();	//获取TCP_Server套接口描述符
	    while(1)	    {
	        struct  sockaddr_in  addr  =  {AF_INET};
	        unsigned  int  addrlen  =  sizeof(addr);
	        int  clifd  =  accept(fd,  (struct  sockaddr*)&addr,  						&addrlen);
	        if(  clifd<0  ) {
	            perror("accept");
	            exit(1);
	        }
while(1) { 	//  发送多个文件
	IPMSG_FILE  *p  =  NULL;
	FILE  *fp  =  NULL;
	IPMSG_USER  temp;
	long  pkgnum  =  0  ;
	char  edition[100]="";  
	long  oldpkgnum  =  0  ;
	long  cmd  =  0;
	int  filenum  =  0;
	char  buf[1400]="";
	int  sendsize  =  0;			
	//接收IPMSG_GETFILEDATA
	 if(recv(clifd,  buf,  sizeof(buf),  0)==0)
	       break;
	       sscanf(buf,%[^:]:%ld:%[^:]:%[^:]:%ld:%lx:%x",edition,  					&pkgnum,  temp.name,  temp.host,  &cmd,
	                 &oldpkgnum,  &filenum);
	  //判断是否为IPMSG_GETFILEDATA          	if((GET_MODE(cmd)&IPMSG_GETFILEDATA)!=IPMSG_GETFILEDATA)
	                 break;
//获取之前发送的文件信息
if  ((p  =  getfileinfo(oldpkgnum,  filenum))==NULL) {
	return  NULL;
}
if(  (fp=fopen(p->name,  "r"))==NULL  ) {
	IPMSG_OUT_MSG_COLOR(
	printf("senderror:  no  such  file:  %s\n",  p->name);
	)
	return  NULL;
}
do {	//发送文件
	int  size  =  fread(buf,  1,  sizeof(buf),  fp);
	isend(clifd,  buf,  size,  0);
	isendsize  +=  size;
}while(sendsize  <  p->size);
	fclose(fp);				//关闭文件
	del_file(p,  SENDFILE);	//从发送文件链表中删除文件
}//end  wile1	  			//循环发送多个文件
	close(clifd);	  	//关闭套接口，等待下一个用户连接
}//end  while
	return  NULL;
}
/****************************************************************
函	数：void  *recv_msg_thread(void  *arg)
功	能：接收消息线程，接收其他客户端发送的UDP数据
参	数：void  *arg
****************************************************************/
void  *recv_msg_thread(void  *arg){
	    while(1)  {
	        char  buf[500]="";		
	        char  edition[100]="";
	        struct  sockaddr_in  addr  =  {AF_INET};
	        unsigned  int  addrlen  =  sizeof(addr);
	        int  len  =  0;
	        long  pkgnum  =  0;
	        long  cmd  =  0;
	        char  msg[100]="";
	        int  t  =  0;
	        char  *p  =  NULL;
	        IPMSG_USER  temp;			
	        len  =  recvfrom(udp_fd(),  buf,  sizeof(buf),  0,  (struct  sockaddr*)&addr,  &addrlen);
	        sscanf(buf,  "%[^:]:%ld:%[^:]:%[^:]:%ld",edition,  &pkgnum,  temp.name,  temp.host,  &cmd);
p  =  strrchr(buf,  ':');			   	//查找附加信息
memcpy(msg,  p+1,  len-(p-buf));	//将附加信息放入msg中
temp.s_addr  =  addr.sin_addr.s_addr;
switch(GET_MODE(cmd)) {
	case  IPMSG_BR_ENTRY:
		t  =  time((time_t)NULL);
		len  =  sprintf(buf,"1:%d:%s:%s:%ld:%s",t,user(),
			      host(),IPMSG_ANSENTRY,user());
		sendto(udp_fd(),buf,len,0,(struct sockaddr*)
		   	   &addr,sizeof(addr));
	case  IPMSG_ANSENTRY:
	         add_user(temp);
	         break;
	case  IPMSG_SENDMSG:
	         if(msg[0]!=0) {
	                  IPMSG_OUT_MSG_COLOR(
	                  printf("\r[recv  msg  from:  %s  ]#\n%s\n",  					temp.name,  msg);
	                  )
	                  write(1,"\rIPMSG:",7);
		}
if((cmd&IPMSG_SENDCHECKOPT)==IPMSG_SENDCHECKOPT) {	
	char  buf[50]="";
	int  len  =  printf(buf,"1:%d:%s:%s:%ld:%ld",t,user(),
			 host(),IPMSG_RECVMSG,  pkgnum);
	                  sendto(udp_fd(),buf,len,0,(struct  sockaddr*)
				   &addr,sizeof(addr)); }
if((cmd&IPMSG_FILEATTACHOPT)==IPMSG_FILEATTACHOPT){
	char  *p  =  msg+strlen(msg)+1;
	//printf("filemsg=%s\n",p);
	char  *fileopt=  strtok(p,  "\a");	//fileopt指向第一个文件属性
	do{	//循环提取文件信息
		IPMSG_FILE  ftemp;
		sscanf(fileopt,  "%d:%[^:]:%lx:%lx",  &ftemp.num,  					  ftemp.name,  &ftemp.size,  &ftemp.ltime);	
	         strcpy(ftemp.user,  temp.name);
	         ftemp.pkgnum  =  pkgnum;
	         add_file(ftemp,  RECVFILE);
	         fileopt  =  strtok(NULL,  "\a"); //fileopt指向下一个文件属性
	                 }while(fileopt!=NULL);
	                 IPMSG_OUT_MSG_COLOR(
	                 printf("\r<<<Recv  file  from  %s!>>>\n", 
				 temp.name);  )
	write(1,"\rIPMSG:",7);
}
break;
case  IPMSG_RECVMSG:
	{
	       IPMSG_OUT_MSG_COLOR(
	       	printf("\r%s  have  receved  your  msg!\n", temp.name);
	       )
	       write(1,"\rIPMSG:",7);
	 }
	 break;
case  IPMSG_BR_EXIT:
	del_user(temp);
	break;
default  :
	break;
}
}
return  NULL;
}