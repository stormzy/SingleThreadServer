#ifndef NET_UTIL_H_H
#define NET_UTIL_H_H

#define ERROR_PRINT(x) \
    printf("desc=%s,file=%s,line=%d,errno=%d,err=%s\n", x, __FILE__, __LINE__, errno, strerror(errno));

extern int StSocket(int type);
extern int StBind(int fd, const char *pszAddr, unsigned short unPort);
extern int StListen(int fd, int backlog);
extern bool StSetNonBlock(int fd);
extern void StSetSocketBuffSize(int fd, int nRcvBuffSize, int nSndBuffSize); 
extern int StConnect(int fd, const char *pszAddr, unsigned short port); 
extern int StAccept(int fd); 

#endif // NET_UTIL_H_H
