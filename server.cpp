#include "common.h"
#include "wrap.h"

void sig_chld(int signo) {
	pid_t		pid;
	int			stat;

	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}

int main()
{
	int						i, maxi, maxfd, listenfd, connfd, sockfd;
	int						nready, client[FD_SETSIZE];
	ssize_t					n;
	fd_set					rset, allset;
	char					buf[MAXLINE];
	
	pid_t					childpid;
	socklen_t				clilen;
	struct sockaddr_in		serv, cli, peer;

	if ((listenfd = socket(IPV4, TCP, 0)) < 0) {
		fprintf(stderr, "%s\n", "socket error.");
		exit(1);
	}
	
	bzero(&serv, sizeof(serv));
	serv.sin_family = IPV4;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port  = htons(SERV_PORT);	
	
	if (bind(listenfd, (struct sockaddr *)&serv, sizeof(serv)) != 0) {
		fprintf(stderr, "%s\n", "bind error.");
		exit(1);
	}

	if (listen(listenfd, MAXCONN) != 0) {
		fprintf(stderr, "%s\n", "listen error.");
		exit(1);
	}
	
	//signal(SIGCHLD, &sig_chld);
	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for ( ; ;) {
		rset = allset;
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);
		
		if (FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cli);
			connfd = Accept(listenfd, (struct sockaddr *)&cli, &clilen);
			socklen_t len = sizeof(peer);
			getpeername(connfd, (struct sockaddr *)&peer, &len);
			char str[INET_ADDRSTRLEN];
			std::cout << "accept "<< inet_ntop(IPV4, &peer.sin_addr, str, sizeof(str)) << std::endl;
			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = connfd;
					break;
				}
			}

			if (i == FD_SETSIZE) {
				fprintf(stderr, "%s\n", "too many clients");
				exit(1);
			}

			FD_SET(connfd, &allset);
			if (connfd > maxfd) 
				maxfd = connfd;
			if (i > maxi) 
				maxi = i;

			if (--nready <= 0)
				continue;
		}

		for (i = 0; i <= maxi; i++) {
			if ( (sockfd = client[i]) < 0 ) 
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
					std::cout << "accept "<< sockfd << std::endl;
				} else {
					Writen(sockfd, buf, n);
				}
				if (--nready <=0)
					break;
			}
		}

	}
	

	return 0;
}
