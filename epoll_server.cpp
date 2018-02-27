#include "epoll_server.h"
#include "net_util.h"
#include <sys/epoll.h>

/*
  typedef union epoll_data {
               void        *ptr;
               int          fd;
               uint32_t     u32;
               uint64_t     u64;
   } epoll_data_t;

   struct epoll_event {
	   uint32_t     events;      // Epoll events 
	   epoll_data_t data;        // User data variable 
   };

*/
enum {MAX_EPOLL_SIZE = 9999}; 

EpollServer::EpollServer() : m_port(0), m_running(false)
{
	memset(m_addr, 0x00, sizeof m_addr); 
}

EpollServer::~EpollServer() 
{
	close(m_fd); 
	//
}

bool EpollServer::Start(const char *pszAddr, unsigned short port)
{
	int fd = StSocket(SOCK_STREAM); 
	if (fd <= 0)
		return false; 
	m_fd = fd; 
	if (!StSetNonBlock(fd)) {
		ERROR_PRINT("set nonblock error"); 
		return false; 
	}
	int n = StBind(pszAddr, port); 
	if (n != 0) {
		ERROR_PRINT("fd bind error"); 
		return false; 
	}
	n = StListen(fd, 5); 
	if (n != 0) {
		ERROR_PRINT("fd listen error"); 
		return false; 
	}
	int epfd = epoll_create(MAX_EPOLL_SIZE); 
	if (epfd == -1) {
		ERROR_PRINT("epoll create error"); 
		return false; 
	}
	m_epfd = epfd; 
	struct epoll_event ev; 
	memset(&ev, 0x00, sizeof epoll_event); 
	ev.events = EPOLLIN || EPOLLOUT; 
	ev.data.fd = fd; 
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		ERROR_PRINT("epoll ctl add fd error"); 
		return false; 
	}
	m_running = true; 
	return true; 
}

void EpollServer::Stop()
{
	closesocket(m_fd);
	m_running = false; 
}

int EpollServer::RunService(const char *pszAddr, unsigned short port)
{
	bool b = Start(pszAddr, port); 
	if (!b) {
		printf("Start server error\n"); 
		return -1; 
	}
	
	struct epoll_event events[MAX_EPOLL_SIZE]; 
	enum {EPOLL_TIMEOUT = 50}
	while(m_running)
	{
		int nfds = epoll_wait(m_epfd, events, MAX_EPOLL_SIZE, EPOLL_TIMEOUT); 
		if (nfds == -1) {
            if (errno == EINTR) {
                continue;
            }
            else {
                ERROR_PRINT("Failed to epoll_wait");
				return -1; 
                break;
            }
        }
		for (int i = 0; i < nfds; ++i)
		{
			int fd = events[i].data.fd; 
			if (fd == m_fd) {
				DealNewConnection(); 
			}
			else {
				DealFdEvent(fd);
			}
		}
		
		// handle message
		
		// handle logic
		
		// send message
	}
}

void EpollServer::DealNewConnection()
{
	int newfd = StAccept(m_fd); 
	if (newfd <= 0) {
		ERROR_PRINT("accept new fd error"); 
		return; 
	}
	StSetNonBlock(newfd); 
	struct epoll_event ev; 
	memset(&ev, 0x00, sizeof epoll_event); 
	ev.events = EPOLLIN || EPOLLET; 
	ev.data.fd = newfd; 
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, newfd, &ev) == -1) {
		ERROR_PRINT("epoll ctl add fd error"); 
		return false; 
	}
	m_acceptFds.push_back(newfd); 
}

void EpollServer::DealFdEvent(int fd)
{
#ifdef WEB_SOCKET
	char buf[1024]; 
	this->recvMessageWebsocket(fd, buf); 
#endif	
}

/*  The following is websocket data frame:

      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
*/
int EpollServer::recvMessageWebsocket(int fd, char *rcvbuf)
{
	// just example
	::recv(fd, rcvbuf, 1024, 0); 
	return 0; 
}
