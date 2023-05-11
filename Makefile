/*************************************************************************
*            文件：      Makefile
*            描述：      Linux操作系统下的Makefile编译文件
*************************************************************************/
obj=main.o user_manager.o communication.o user_interface.o file_manager.o
CC=gcc
CFLAGS=-Wall -O2
execfile=ipmsg
$(execfile):$(obj)
	     $(CC) -o $@ $^ $(CFLAGS) -lpthread

%O:%C %h
	     $(CC) -c -o $@ $^ $(CFLAGS)
clean:
	@rm $(obj) -rf $(execfile) –rf
