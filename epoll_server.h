#ifndef EPOLL_SERVER_H_H
#define EPOLL_SERVER_H_H

#include <vector>
using std::vector; 

class EpollServer
{
public:
	EpollServer(); 
	~EpollServer();
	void Stop();
	int RunService(const char *pszAddr, unsigned short port); 
private:
	bool Start(const char *pszAddr, unsigned short port); 
	void DealNewConnection(); 
	void DealFdEvent(int fd); 
	void DealOtherSrvEvent(int fd); 
	int recvMessageWebsocket(int fd); 
private:
	char m_addr[64]; 
	unsigned short m_port;
	bool m_running; 
	int m_fd; 
	int m_epfd; 
	vector<int> m_acceptFds;
};

#endif //  EPOLL_SERVER_H_H
