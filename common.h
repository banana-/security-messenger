#ifndef __COMMON_H
#define __COMMON_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "const.h"

void str_echo(int);
void str_cli(FILE *, int);

#endif
