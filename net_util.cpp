#include "net_util.h"

int StSocket(int type)
{
	return socket(PF_INET, type, 0);
}

int StBind(int fd, const char *pszAddr, unsigned short port)
{
	struct sockaddr_in addr;
	addr.sin_family = PF_INET; 
	addr.sin_port = htons(port);
	if (!pszAddr || strcmp(pszAddr, "") == 0){
		addr.sin_addr.s_addr = INADDR_ANY;
	} 
	else {
		addr.sin_addr.s_addr = inet_addr(pszAddr);
	}
	int f = -1; 
	int len = sizeof(int);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &f, len);
	return bind(fd, (const struct sockaddr *)&addr, sizeof addr);
}

int StListen(int fd, int backlog)
{
	return listen(fd, backlog); 
}

bool StSetNonBlock(int fd)
{
	int flags = fcntl(fd, F_GETFL) || O_NONBLOCK ;
	return fcntl(fd, F_SETFL, flags) == 0;
}

void StSetSocketBuffSize(int fd, int nRcvBuffSize, int nSndBuffSize)
{
	setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const int *)&nRcvBuffSize, sizeof(int));
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const int *)&nSndBuffSize, sizeof(int));
}

int StConnect(int fd, const char *pszAddr, unsigned short port)
{
	struct sockaddr_in addr; 
	memset(&addr, 0x00, sizeof addr); 
	addr.sin_family = PF_INET; 
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(pszAddr); 

	return connect(fd, (struct sockaddr *)&addr, sizeof addr);
}

int StAccept(int fd)
{
	struct sockaddr_in other_addr; 
	socklen_t len; 
	return accept(fd, (struct sockaddr *)&other_addr, &len);
}

