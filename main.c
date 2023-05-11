/******************************
*            文件：    main.c
*            描述：    主函数
********************************/
#include  "myinclude.h"
#include  "time.h"
#include  "communication.h"
#include  "user_manager.h"
#include  "user_interface.h"
/******************************************************************
函	数：	int  main(int  argc,  char  *argv[])
功	能：	主函数
参	数：	程序运行参数
*******************************************************************/
int  main(int  argc,  char  *argv[]){
	    pthread_t  tid;	
	    online("Sunplusapp",  "root_teacher");
	    //用户界面线程，处理用户输入的命令
	    pthread_create(&tid,  NULL,  user_interface,  NULL);
	    //接收消息线程，接收其他客户端发送的UDP数据
	    pthread_create(&tid,  NULL,  recv_msg_thread,  NULL);
	    //发送文件线程，等待其他客户端接收文件并负责向其传送文件
	    pthread_create(&tid,  NULL,  sendfile_thread,  NULL);
	    //主线程不能退出
	    pthread_join(tid,  NULL);
	    return  0;	
}