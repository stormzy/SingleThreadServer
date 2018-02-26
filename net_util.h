#ifndef NET_UTIL_H_H
#define NET_UTIL_H_H

extern int StSocket(int type);
extern bool StBind(int fd, const char *pszAddr, unsigned short unPort);
extern int StListen(int fd, int backlog);
extern bool StSetNonBlock(int fd);
extern void StSetSocketBuffSize(int fd, int nRcvBuffSize, int nSndBuffSize); 


#endif // NET_UTIL_H_H
