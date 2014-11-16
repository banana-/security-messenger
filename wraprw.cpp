#include "common.h"

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char  *ptr;

	ptr = (const char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return (-1);
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}


ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nread;
	char		*ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if ( errno == EINTR)
				nread = 0;
			else
				return (-1);
		} else if (nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}	

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t		n, rc;
	char		c, *ptr;

	ptr = (char *)vptr;
	for (n = 1; n < maxlen; n++) {
		again:
			if ( (rc = read(fd, &c, 1)) == 1) {
				*ptr++ = c;
				if (c == '\n')
					break;
			} else if (rc == 0) {
				*ptr = 0;
				return (n - 1);
			} else {
				if (errno == EINTR)
					goto again;
				return (-1);
			}
	}
	*ptr = 0;
	return (n);
}

void str_echo(int sockfd)
{
	ssize_t			n;
	char			buf[MAXLINE];
	
again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
		if (writen(sockfd, buf, n) < 0) {
			fprintf(stderr, "%s\n", "writen error.");
			exit(1);
		}	
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0) {
		fprintf(stderr, "%s\n", "str_echo: read error");
		exit(1);
	}
}

void str_cli(FILE *fp, int sockfd) 
{
	char	sendline[MAXLINE], recvline[MAXLINE];
	int		maxfdp1;
	fd_set rset;

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = std::max(fileno(fp), sockfd)+1;
		
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "%s\n", "str_cli: select erro");
			exit(1);
		}

		if (FD_ISSET(sockfd, &rset)) {
			if (readline(sockfd, recvline, MAXLINE) <= 0) {
				fprintf(stderr, "str_cli: server terminated prematurely.");
				exit(1);
			}
			if (fputs(recvline, stdout) == EOF) {
				fprintf(stderr, "%s\n", "fputs error");
				exit(1);
			}
		}

		if (FD_ISSET(fileno(fp), &rset)) {
			if(fgets(sendline, MAXLINE, fp) == NULL)
				return ;
			writen(sockfd, sendline, strlen(sendline));
		}
		
	}
}
