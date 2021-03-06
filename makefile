servobjs = server.o $(commonobjs) 
cliobjs = client.o $(commonobjs)

commonobjs = wraprw.o common.o

cm = g++ -g

all : client server
	
client : $(cliobjs)
	$(cm) -o client $(cliobjs)	

server : $(servobjs)
	$(cm) -o server $(servobjs)

client.o : client.cpp common.h const.h
	$(cm) -c client.cpp

server.o : server.cpp common.h const.h
	$(cm) -c server.cpp

wraprw.o : wraprw.cpp common.h const.h
	$(cm) -c wraprw.cpp

common.o : common.cpp common.h
	$(cm) -c common.cpp

.PHONY: clean
clean :
	-rm *.o
