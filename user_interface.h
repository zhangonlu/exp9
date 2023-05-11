/**********************************************************
*            文件：      user_interface.h
*            描述：      用户管理头文件
***********************************************************/
#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_
//用户界面线程，处理用户输入的命令
void *user_interface(void *arg);
#endif	//_USER_INTERFACE_H_
/**************************************************************
*            文件：      user_manager.c
*            描述：      用户管理
*************************************************************/
#include  "myinclude.h"
#include  "user_manager.h"
static  IPMSG_USER  *head  =  NULL;
/**************************************************************
函	数：void  add_user(IPMSG_USER  temp)
功	能：向用户列表中添加一个用户结点
参	数：IPMSG_USER  temp
**************************************************************/
void  add_user(IPMSG_USER  temp){
	IPMSG_USER  *pf=head,  *pb=NULL,  *pend=NULL;
	while(pf  !=  NULL)	    {
	if(pf->s_addr  ==  temp.s_addr) //用户是否已经在链表中
	         return;
	         pend  =  pf;
	         pf  =  pf->next;	
	}
pb=(IPMSG_USER  *)malloc(sizeof(IPMSG_USER));
strcpy(pb->name,  temp.name);
	strcpy(pb->host,  temp.host);
	pb->s_addr  =  temp.s_addr;	
	pb->next=NULL;
 	if(head  ==  NULL)
	 	head  =  pb;
	else
		pend->next  =  pb;
}
/**********************************************************
函	数：del_user(IPMSG_USER  user)
功	能：从用户列表中删除一个用户结点
参	数：IPMSG_USER  user
***********************************************************/
void  del_user(IPMSG_USER  user){
	 IPMSG_USER  *p1=head,*p2=NULL;
	 if(head==NULL)      return  ;
	 else {
	         while((strcmp(p1->name,user.name)!=0)
			&&(p1->next!=NULL)) {
	            		 p2=p1;
	            		 p1=p1->next;
	         	}
	         if(strcmp(p1->name,user.name)==0) {
	            if(p1==head)
	                head=p1->next;
	            else  
	                p2->next=p1->next;
	            free(p1);
	         }
	    }
}
/****************************************************************
函	数：int  get_addr_by_name(char  *name)
功	能：根据用户名返回IP地址
参	数：char  *name
****************************************************************/
int  get_addr_by_name(char  *name){
       IPMSG_USER  *p=head;
	while(p!=NULL)   {
	     if(strcmp(p->name,  name)==0)  { return  p->s_addr; }
	       	 p  =  p->next;
	      }	
	    return  0;
}
/******************************************************************
函	数：void  list(void)
功	能：打印用户列表
参	数：无
*******************************************************************/
void  list(void){
	 IPMSG_USER  *p=head;
	 IPMSG_OUT_MSG_COLOR(
	 printf("%10s\t%10s\t%s\n",  "Username","Hostname",
		"IP  Address");   )
	    while(p!=NULL) {
	        struct  in_addr  addr  =  {p->s_addr};
	        IPMSG_OUT_MSG_COLOR(
	        printf("%10s\t%10s\t%s\n",p->name,p->host,
			inet_ntoa(addr));)
	        p=p->next;
	    }	
}
/********************************************************************
*            文件：     user_manager.h
*            描述：     用户管理头文件
********************************************************************/
#ifndef _LIST_H_
#define _LIST_H_
//用户信息结构体
typedef struct userlist{
	    char name[20];	//用户名
	    char host[20];		//主机名
	    int s_addr;		//IP地址(32位网络字节序)
	    struct userlist *next;
}IPMSG_USER;
//向用户列表中添加一个用户结点
void add_user(IPMSG_USER temp);
//从用户列表中删除一个用户结点
void del_user(IPMSG_USER temp);
//根据用户名返回IP地址
int get_addr_by_name(char *name);
//打印用户列表
void list(void);
//void free_link(void);
#endif	//_LIST_H_