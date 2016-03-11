/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 17:27:55 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/11 19:30:24 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

typedef unsigned char u4;
typedef unsigned short u8;
typedef unsigned int u16;
typedef unsigned long u32;

struct s_ip {
	u4	ip_v;      // Must be IPVERSION (4);
	u4	ip_hl;     // Between 5 and 10 (inclusive);
	u8	ip_tos;    // Any value;
	u16	ip_len;    // Must be the total length of IP datagram (IP header + ICMP message);
	u16	ip_id;     // Must be zero, will be automatically set;
	u16	ip_off;    // Must be zero, will be automatically set;
	u8	ip_ttl;    // Any value;
	u8	ip_p;      // Must be IPPROTO_IP;
	u16	ip_sum;    // Value ignored, will be automatically set;
	u32	ip_src;    // Must be an IP address currently assigned to one of the local interface or INADDR_ANY;
	u32	ip_dst;    // Any address;
	//	ip_opts   // Any option.
}	typedef t_ip;

struct s_icmp {
	u8	type;
	u8	code;
	u16	checksum;
}	typedef t_icmp;

int main(int argc, char **argv)
{
	/*
	   printf("u4: %lu\n", sizeof(u4));
	   printf("u8: %lu\n", sizeof(u8));
	   printf("u16: %lu\n", sizeof(u16));
	   printf("u32: %lu\n", sizeof(u32));
	   */
	int s;

	socklen_t dest_len;
	struct protoent *s_p;
	struct addrinfo hints;
	struct addrinfo *res;

	s_p = getprotobyname("icmp");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_RAW;

	if (getaddrinfo(argv[1], argv[2], &hints, &res) == -1)
	{
		printf("Error getaddrinfo");
		exit(1);
	}

	// Socket
	printf("t_ip: %lu\n", sizeof(t_ip));
	char buf[400];
	struct ip *ip = (struct ip*)buf;
	struct icmphdr *icmp;
//	ip->ip_v = 4;
//	ip->ip_hl = 3;
//	ip->ip_len = 200;
/*
	if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		printf("Error getaddrinfo");
		exit(1);
	}
*/
	printf("Socket: %d\n", s);


	return (0);
}
