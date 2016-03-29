/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/29 14:48:02 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/29 16:23:13 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

struct s_icmp {
	u8	type;
	u8	code;
	u16	cheksum;
}	typedef t_icmp;

/*
struct s_boolean {
	u8 boolean: 1;
}	typedef t_boolean;
*/

int main(int argc, char **argv)
{
/*
	t_boolean flag;

	flag.boolean = 4;
	
	printf("Flag: %u\n", flag.boolean);

	printf("Boolean: %lu\n", sizeof(t_boolean));
	printf("char: %lu\n", sizeof(unsigned char));
	printf("short: %lu\n", sizeof(unsigned short));
	printf("int: %lu\n", sizeof(unsigned int));
	printf("long %lu\n", sizeof(unsigned long));
*/


	int s;

	struct addrinfo hints;
	struct addrinfo *res;

	if (argc != 3)
	{
		printf("Usage: %s <addres ip dst> <address ip src>\n", argv[0]);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	// GETADDRINFO

	if (getaddrinfo(argv[1], NULL, &hints, &res) < 0)
	{
		perror("Error getaddrinfo");
		exit(1);
	}

	// SOCKET

	if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
	{
		perror("Error socket():");
	}

	int hincl;

	hincl = 1;

	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl)) < 0)
	{
		perror("Error setsockopt");
		exit(1);
	}

	// SEND
	char buf[64];
	struct ip *ip = (struct ip *)buf;
	struct icmp *icmp = (struct icmp *)(ip + 1);
	
	bzero(buf, sizeof(buf));

	ip->ip_v = 4;
	ip->ip_hl = sizeof(*ip) >> 2; // 5 * 4 = 20; 5 words of 32bits
	ip->ip_tos = 0;
	ip->ip_len = htons(sizeof(buf));
	ip->ip_ttl = 64;
	ip->ip_p = res->ai_protocol;
	ip->ip_sum = 0;

	inet_pton(res->ai_family, argv[2], &(ip->ip_src.s_addr));
	inet_pton(res->ai_family, argv[1], &(ip->ip_dst.s_addr));

	printf("IP Version: %u\n", ip->ip_v);
	printf("Sizeof ip: %lu\n", sizeof(*ip));
	printf("IP HL: %u\n", ip->ip_hl);

	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = htons(~(ICMP_ECHO << 8));

	int nb_send;

	if ((nb_send = sendto(s, buf, sizeof(buf), 0, res->ai_addr, res->ai_addrlen)) < 0)
	{
		perror("Error sendto");
		exit(1);
	}

	// RECEIVE
	int nb_receive;
	struct msghdr msg;
	struct iovec iov[1];
	char bufControl[1000];

	bzero(buf, sizeof(buf));

	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof(buf);

	msg.msg_name = res->ai_addr;
	msg.msg_namelen = res->ai_addrlen;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = &bufControl;
	msg.msg_controllen = sizeof(bufControl);
	msg.msg_flags = 0;

	if ((nb_receive = recvmsg(s, &msg, 0)) < 0)
	{
		perror("Error recmsg: ");
		exit(1);
	}

	printf("Ret: %d\n", nb_receive);
	printf("Type: %d\n", icmp->icmp_type);
	printf("Code: %d\n", icmp->icmp_code);

	return (0);
}
