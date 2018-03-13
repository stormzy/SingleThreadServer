# Notes: 
# $@ -- target file
# $< -- the first required file
# $^ -- all required files

CXX = g++

PRO_NAME=main

CXXFLAGS = -c -g -std=c++0x 
OBJS = main.o epoll_server.o net_util.o
$(PRO_NAME):$(OBJS)
	$(CXX) -o $@ $^
.cpp.o:
	$(CXX) $(CXXFLAGS) $<

.PHONY : clean
clean :
	rm -f $(PRO_NAME) $(OBJS)
