/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/10 15:27:02 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/11 19:31:55 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int s;
	
	socklen_t dest_len;
	struct protoent *s_p;
	struct addrinfo hints;
	struct addrinfo *res;

	s_p = getprotobyname("icmp");
	printf("%s\n", s_p->p_name);
	printf("%d\n", s_p->p_proto);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((getaddrinfo(argv[1], argv[2], &hints, &res) == -1))
	{
		printf("Error getaddrinfo");
		exit(1);
	}

	printf("Socktype: %d\n", res->ai_socktype);
	printf("Protocol: %d\n", res->ai_protocol);

	if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		printf("Error getaddrinfo");
		exit(1);
	}

	printf("Socket: %d\n", s);

	/*Send Part*/
	char *msg = "Hello";
	char buffer[1024] = {0};
	//struct msghdr s_msg;
	int flags;
	int read_len;

	printf("Len: %d\n", res->ai_addrlen);
	printf("Len msg: %lu\n", strlen(msg));
	sendto(s, argv[3], strlen(argv[3]), flags, res->ai_addr, res->ai_addrlen);
	
	/* Recevie Part */
	struct msghdr s_msg;
	int r_flags;
	ssize_t read;

	read = recvmsg(s, &s_msg, r_flags);

	printf("Recvmsg: %zd", read);

//	printf("Len msg_namelen: %d", s_msg->msg_namelen);
	
	return (0);
}
