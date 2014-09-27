objs = server.o
cm = g++ -g

server : $(objs)
	$(cm) -o server $(objs)

server.o : server.cpp common.h
	$(cm) -c server.cpp

.PHONY: clean
clean :
	-rm $(objs)
