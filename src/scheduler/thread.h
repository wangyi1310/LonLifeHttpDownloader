#ifndef _THREAD_H_
#define _THREAD_H_
#include <stdbool.h> 
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "md5.h"
#include "pipe.h"
#include "../json/light_json.h"
#include "../memory/alloc.h"
#include "../log/log.h"
#include "algorithm.h"
#include <fcntl.h>
#include <sys/epoll.h>
typedef struct Argument{
	Pipe *ppp;
	//schduler related
	FdSet *fd_set;	
}Argument;

// main thread
void getting_connect(int fd, Pipe *ppp);
// child thread
void *dealing_io(void *);
void set_no_blocking(int fd);
#endif
