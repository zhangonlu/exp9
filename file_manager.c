/*********************************************************
*            文件：      file_manager.c
*    		 描述： 发送文件信息*********************************************************/
#include  <sys/stat.h>
#include  "myinclude.h"
#include  "file_manager.h"
static  IPMSG_FILE  *sendhead  =  NULL;	//发送链表
static  IPMSG_FILE  *recvhead  =  NULL;  	//接收链表
/******************************************************************
函	数：void  add_file(IPMSG_FILE  temp,  int  flag)
功	能：向链表（接收或发送链表）中添加一个文件
参	数：IPMSG_FILE temp, int  flag，flag包含SENDFILE（发送链表）和RECVFILE（接收链表)
*******************************************************************/
void  add_file(IPMSG_FILE  temp,  int  flag){
	    IPMSG_FILE  **phead=NULL,  *pf=NULL,  *pb=NULL;
	    phead  =  (flag==SENDFILE)?(&sendhead):(&recvhead);
	    pf  =  *phead;
	    pb  =  (IPMSG_FILE  *)malloc(sizeof(IPMSG_FILE));
	    strcpy(pb->name,  temp.name);
	    strcpy(pb->user,  temp.user);
	    pb->num  =  temp.num;
	    pb->ltime  =  temp.ltime;
	    pb->size  =  temp.size;
	    pb->pkgnum  =  temp.pkgnum;
	    pb->next  =  NULL;
if(  *phead==NULL  )	    {	 *phead  =  pb;	    }
else    {
	 while(pf->next  !=  NULL) {  pf  =  pf->next;    }
	  pf->next  =  pb;
    }
}
/*********************************************************
函	数：void  del_file(IPMSG_FILE  *temp,  int  flag)
功	能：从链表（接收或发送链表）中删除一个文件
参	数：IPMSG_FILE  temp,  int  flag， flag包含SENDFILE（发送链表）和RECVFILE（接收链表） 
**********************************************************/
void  del_file(IPMSG_FILE  *temp,  int  flag){
	    IPMSG_FILE  **phead=NULL,  *pf=NULL,  *pb=NULL;
	    phead  =  (flag==SENDFILE)?&sendhead:&recvhead;
	    pb  =  *phead;
	    while(pb!=NULL)	    {
	        if(  (temp->pkgnum==pb->pkgnum)  &&  (temp->num==pb->num)  ) {
	            if(pb==*phead)     *phead  =  pb->next;	
	            else		         pf->next  =  pb->next;
  		   pf=pb->next;
	            free(pb);
	return  ;}
	pf  =  pb;
	pb  =  pb->next; }
}
/***************************************************************
函	数：IPMSG_FILE  *getfileinfo(long  pkgnum,  int  filenum)
功	能：根据包标号和文件序号从发送链表中获取文件
参	数：long  pkgnum,  int  filenum
****************************************************************/
IPMSG_FILE  *getfileinfo(long  pkgnum,  int  filenum){
	    IPMSG_FILE  *p  =  sendhead;
	    while(p!=NULL)   {
	        if(  p->pkgnum  !=  pkgnum  )   {
	            p  =  p->next;
	            continue;
	        }
	        if(  p->num  !=  filenum  )  {	
	            p  =  p->next;
	            continue;
	        }
	        return  p;
	    }
	    return  NULL;}
/***************************************************************************************
函	数：int  getfileopt(char  *fileopt,  char  *filename,  long  pkgnum,  int  num)
功	能：获取文件属性并保存到fileopt中，将文件信息保存到发送链表中
参	数：char  *fileopt,  char  *filename,  long  pkgnum,  int  num 
*****************************************************************************************/
int  getfileopt(char  *fileopt,  char  *filename,  long  pkgnum,  int  num){
	    int  len  =  0;
	    IPMSG_FILE  file;
	    struct  stat  fstat;	
	    int  ret  =  stat(filename,  &fstat);
	    if(  ret<0  ) {	
	        IPMSG_OUT_MSG_COLOR(
	        printf("No  such  file:*%s*!\n",  filename);
	        )
	        return  0;
	    }
	    strcpy(file.name,  filename);
	    file.num  =  num;
	    file.pkgnum  =  pkgnum;
	    file.size  =  fstat.st_size;
	    add_file(file,  SENDFILE);
	    len  =  sprintf(fileopt,  "%d:%s:%lx:%lx:%ld\a",  \file.num,  file.name,  					fstat.st_size,  fstat.st_ctime,IPMSG_FILE_REGULAR  );
	    return  len;
}
/***********************************************
函	数：IPMSG_FILE  *  find_file(int  id)
功	能：在接收链表中按照序号（id）查找文件
参	数：int id 
***********************************************/
IPMSG_FILE  *  find_file(int  id)
{	
	    int  i  =  id;
	    IPMSG_FILE  *p  =  recvhead;
	    if(p  ==  NULL)
	         return  NULL;
	    for(i=0;i!=id;i++)
	    {
	         if(  p->next  ==  NULL  )
	         {
	             return  NULL;
	    }
	         p  =  p->next;
	    }	
	    return  p;
}
/****************************************************************
函	数：void  file_list(void)
功	能：打印接收文件列表
参	数：无
****************************************************************/
void  file_list(void){
	    int  id  =  0;
	    IPMSG_FILE  *pb  =  recvhead;
	    IPMSG_OUT_MSG_COLOR(
	    printf("\rNUM\t\tFILE\t\tSender\n");   )
	    while(pb  !=  NULL) {
	        IPMSG_OUT_MSG_COLOR(
	        printf("%d\t%20s\t%s\n",  id++,  pb->name,  pb->user);
	        )
	        pb  =  pb->next;    }
}