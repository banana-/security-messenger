#include "common.h"

int Select(int maxfd, fd_set *rset, fd_set *wset, fd_set *eset, struct timeval *timeout) 
{
	int n = select(maxfd, rset, wset, eset, timeout);
	if ( n <= 0 ) {
		fprintf(stderr, "%s\n", "select error.");
		exit(1);
	}
	return n;
}

int Accept(int listenfd, struct sockaddr *cli, socklen_t *clilen) 
{
	int connfd = accept(listenfd, cli, clilen);
	if (connfd == -1) {
		fprintf(stderr, "%s\n", "accept error.");
		exit(1);
	}

	return connfd;
}


int Read(int sockfd, char *buf, int buf_len) {
	int n = read(sockfd, buf, buf_len);
	if (n < 0) {
		fprintf(stderr, "%s\n", "read error.");
		exit(1);
	}
	
	return n;
}

void Close(int fd) 
{
	if (close(fd) == -1) {
		fprintf(stderr, "%s\n", "close error.");
		exit(1);
	}
}

void Writen(int fd, char *buf, int len) 
{
	if (write(fd, buf, len) == -1) {
		fprintf(stderr, "%s\n", "write error.");
		exit(1);
	}
}
