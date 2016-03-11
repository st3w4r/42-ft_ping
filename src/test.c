/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/07 17:06:56 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/10 17:09:37 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int s;
	struct protoent *s_p;
	struct addrinfo hints;
	struct addrinfo *res;

	s_p = getprotobyname("icmp");
	printf("%s\n", s_p->p_name);
	printf("%d\n", s_p->p_proto);
	printf("%d\n", AF_INET);

	if ((s = socket(PF_INET, SOCK_DGRAM, s_p->p_proto)) == -1)
	{
		printf("Error getaddrinfo");
		exit(1);
	}

	printf("%d\n", s);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_ALL;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	if (getaddrinfo(argv[1], argv[2], &hints, &res) == -1)
	{
		printf("Error getaddrinfo");
		exit(1);
	}

	char *msg = "Hello";
	char buffer[1024];
	struct msghdr s_msg;
	socklen_t dest_len;
	int flags;
	int read_len;

	printf("Len %lu\n", strlen(msg));

	sendto(s, msg, strlen(msg), MSG_OOB, (struct sockaddr*)&hints, (int)sizeof(hints));

//	read_len = recvfrom(s, buffer, sizeof(buffer) -1, 0, (struct sockaddr*)res, sizeof(struct sockaddr));
	read_len = recvmsg(s, &s_msg, flags);

	printf("Read len: %d", read_len);
}
