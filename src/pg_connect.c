/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/07 16:03:51 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/31 17:31:24 by ybarbier         ###   ########.fr       */
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
		ft_error_str_exit("Error getaddrinfo");
	if ((env->s = socket(env->res->ai_family, env->res->ai_socktype,
		env->res->ai_protocol)) < 0)
		ft_error_str_exit("Error socket opening");
	if (setsockopt(env->s, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl)) < 0)
		ft_error_str_exit("Error setsocket");
	return (env->s);
}

void	pg_configure_header(t_env *env)
{
	struct ip	*ip = (struct ip *)env->buf;
	struct icmp	*icmp = (struct icmp *)(ip + 1);
	
	ft_memset(&(env->buf), 0, sizeof(env->buf));
	ip->ip_v = 4;
	ip->ip_hl = sizeof(*ip) >> 2;
	ip->ip_tos = 0;
	ip->ip_len = htons(sizeof(env->buf));
	ip->ip_ttl = 64;
	ip->ip_p = env->res->ai_protocol;
	ip->ip_sum = 0;
	inet_pton(env->res->ai_family, env->host_src, &(ip->ip_src.s_addr));
	inet_pton(env->res->ai_family, env->host_dst, &(ip->ip_dst.s_addr));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = htons(~(ICMP_ECHO << 8));
}
