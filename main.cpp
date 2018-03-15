#include "epoll_server.h"
#include "net_util.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Go In procdure!\n");
    EpollServer svr;
    svr.RunService("127.0.0.1", 12324); 
	printf("Go Out procedure!\n");
	return 0;
}
