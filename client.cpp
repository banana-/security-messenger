#include "common.h"

int main(int argc, char **argv)
{
	int						sockfd[5];
	struct sockaddr_in		servaddr;

	if (argc != 2) {
		fprintf(stderr, "%s\n", "usage: tcpcli <IPaddress> ");
		exit(1);
	}
	int i;
	for (i=0; i<5; ++i) {
		sockfd[i] = socket(IPV4, TCP, 0);

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = IPV4;
		servaddr.sin_port = htons(SERV_PORT);
		if (inet_pton(IPV4, argv[1], &servaddr.sin_addr) != 1) {
			fprintf(stderr, "%s%s\n", "invalid address: ", argv[1]);
			exit(1);
		}

		if (connect(sockfd[i], (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
			fprintf(stderr, "%s\n", "connect error.");
			exit(1);
		}

	}
	str_cli(stdin, sockfd[0]);

	return 0;
}
