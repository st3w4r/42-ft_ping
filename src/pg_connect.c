/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/07 16:03:51 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/07 15:44:25 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int		pg_open_socket(t_env *env)
{
	int hincl;

	hincl = 1;
	ft_memset(&(env->hints), 0, sizeof(env->hints));
	env->hints.ai_family = AF_INET;
	env->hints.ai_socktype = SOCK_RAW;
	env->hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(env->host_dst, NULL, &(env->hints), &(env->res)) < 0)
		ft_error_str_exit("Error getaddrinfo\n");
	if ((env->s = socket(env->res->ai_family, env->res->ai_socktype,
		env->res->ai_protocol)) < 0)
		ft_error_str_exit("Error socket opening\n");
	if (setsockopt(env->s, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl)) < 0)
		ft_error_str_exit("Error setsocket\n");

	return (env->s);
}

void	pg_configure_header(t_env *env)
{
	env->ip = (struct ip *)env->buf;
	env->icmp = (struct icmp *)(env->ip + 1);
}

void	pg_configure_send(t_env *env, unsigned short id, unsigned short seq)
{
	ft_memset(&(env->buf), 0, sizeof(env->buf));
	env->ip->ip_v = 4;
	env->ip->ip_hl = sizeof(*(env->ip)) >> 2;
	env->ip->ip_tos = 0;
	env->ip->ip_len = htons(sizeof(env->buf));
	env->ip->ip_id = 0;
	env->ip->ip_off |= htons(IP_DF);
	env->ip->ip_ttl = 64;
	env->ip->ip_p = env->res->ai_protocol;
	env->ip->ip_sum = 0;
	inet_pton(env->res->ai_family, env->host_src, &(env->ip->ip_src.s_addr));
	inet_pton(env->res->ai_family, env->host_dst, &(env->ip->ip_dst.s_addr));
	env->icmp->icmp_type = ICMP_ECHO;
	env->icmp->icmp_code = 0;
	env->icmp->icmp_hun.ih_idseq.icd_id = id;
	env->icmp->icmp_hun.ih_idseq.icd_seq = seq;
	env->icmp->icmp_cksum = htons(pg_icmp_checksum(env->icmp->icmp_type,
		env->icmp->icmp_code, htons(id), htons(seq)));
}

void	pg_configure_receive(t_env *env)
{
	ft_memset(&(env->buf), 0, sizeof(env->buf));
	env->iov[0].iov_base = env->buf;
	env->iov[0].iov_len = sizeof(env->buf);
	env->msg.msg_name = env->res->ai_addr;
	env->msg.msg_namelen = env->res->ai_addrlen;
	env->msg.msg_iov = env->iov;
	env->msg.msg_iovlen = 1;
	env->msg.msg_control = &(env->bufControl);
	env->msg.msg_controllen = sizeof(env->bufControl);
	env->msg.msg_flags = 0;
}
