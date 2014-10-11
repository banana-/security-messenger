#include "common.h"

void sig_chld(int signo) {
	pid_t		pid;
	int			stat;

	
	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}

int main()
{
	int						listenfd, connfd;
	pid_t					childpid;
	socklen_t				clilen;
	struct sockaddr_in		serv, cli;

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
	
	signal(SIGCHLD, &sig_chld);

	for ( ; ;) {
		clilen = sizeof(cli);
		connfd = accept(listenfd, (struct sockaddr *) &cli, &clilen);
		if (-1 == connfd) {
			fprintf(stderr, "%s\n", "accept error.");
			exit(1);
		}
		
		if ( (childpid = fork()) < 0) {
			fprintf(stderr, "%s\n", "fork error.");
			exit(1);
		} else if (0 == childpid) {
			if (-1 == close(listenfd)) {
				fprintf(stderr, "%s\n", "close error.");
				exit(1);
			}
			str_echo(connfd);
			exit(0);
		} else {
			std::cout << "start server successfully." << std::endl;
		}
	}
	

	return 0;
}
