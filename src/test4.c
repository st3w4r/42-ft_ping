/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 17:27:55 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/25 18:45:07 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

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
	if (argc != 3)
	{
		printf("Usage: %s <adresse ip dst> <adresse ip src>\n", argv[0]);
		exit(1);
	}
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
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if (getaddrinfo(argv[1], NULL, &hints, &res) < 0)
	{
		perror("Error getaddrinfo");
		exit(1);
	}

	// Socket
	printf("t_ip: %lu\n", sizeof(t_ip));
//	char buf[400];
//	struct ip *ip = (struct ip*)buf;
//	struct icmphdr *icmp;
//	ip->ip_v = 4;
//	ip->ip_hl = 3;
//	ip->ip_len = 200;

	printf("Family: %d\n", hints.ai_family);
	printf("Socktype: %d\n", hints.ai_socktype);
	printf("Protocol: %d\n", hints.ai_protocol);
	printf("Family: %d\n", res->ai_family);
	printf("Socktype: %d\n", res->ai_socktype);
	printf("Protocol: %d\n", res->ai_protocol);

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
	{
		perror("socket() error");
		exit(1);
	}


	printf("Socket: %d\n", s);

	int hincl;

	hincl = 1;
	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl)) < 0)
	{
		perror("setscokopt() error");
		exit(1);
	}

	char buf[64];
	// t_ip *ip = (t_ip *)buf;
	struct ip *ip = (struct ip *)buf;
	t_icmp *icmp = (t_icmp *)(ip + 1);

	struct sockaddr_in dst;

	bzero(buf, sizeof(buf));

	ip->ip_v = 4;
	ip->ip_tos = 0;
	ip->ip_hl = sizeof(*ip) >> 2;
	ip->ip_ttl = 64;
	ip->ip_len = htons(sizeof(buf));
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;

	ip->ip_dst.s_addr = inet_addr(argv[1]);
	ip->ip_src.s_addr = inet_addr(argv[2]);

	dst.sin_addr = ip->ip_dst;
	dst.sin_family = PF_INET;
	printf("%s\n", inet_ntoa(ip->ip_dst));
	printf("%s\n", inet_ntoa(ip->ip_src));

	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	// icmp->checksum = htons(~(ICMP_ECHO << 8));
	icmp->checksum = 65527;
	printf("Checksum: %d\n", icmp->type);
	printf("Checksum: %d\n", ICMP_ECHO);
	printf("Checksum: %d\n", ICMP_ECHO << 8);
	printf("Checksum: %d\n", ~(ICMP_ECHO << 8));
	printf("Checksum: %d\n", htons(~(ICMP_ECHO << 8)));

	printf("Checksum: %u\n", ntohs(icmp->checksum));
	printf("Checksum: %u\n", ntohs(~icmp->checksum));
	printf("Checksum: %u\n", (ntohs((~icmp->checksum))) >> 8);

	printf("Test: %u\n", ntohs(1));

	printf("Buf: %lu\n", sizeof(buf));
	// for (int i = 0; i < sizeof(buf); i++)
	// {
	// 	printf("%01X:", buf[i]);
	// }
	// exit(1);

	int nb_send;

	if ((nb_send = sendto(s, buf, sizeof(buf), 0, res->ai_addr, res->ai_addrlen)) < 0)
	{

	// if ((nb_send = sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst))) < 0)
	// {
		perror("Error sendto");
		exit(1);
	}


	// Reicive
	int s_rec;
	s_rec = 0;
	if ((s_rec = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		perror("socket() error");
		exit(1);
	}

	int ret;
	struct msghdr msg;
	struct iovec iov[1];
	//char bufRec[400];
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

	ret = recvmsg(s_rec, &msg, 0);
//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
//		                 struct sockaddr *src_addr, socklen_t *addrlen);

//	ret = recvfrom(s, buf, sizeof(buf), 0, res->ai_addr, &res->ai_addrlen);

	if (ret < 0)
		perror("Error recvmsg()");

	printf("Ret: %d\n", ret);
	printf("Type: %d\n", icmp->type);
	printf("Code: %d\n", icmp->code);
	return (0);
}

