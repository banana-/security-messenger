#include "common.h"

int main()
{
	int						listenfd;
	struct sockaddr_in		serv;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port  = htons(SERV_PORT);	

	
	std::cout << "starting server..." << std::endl;
	
		

	return 0;
}
