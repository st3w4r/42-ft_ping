/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/09 15:26:01 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/09 18:39:19 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	pg_display_info(t_env *env)
{
	printf("PING %s (%s): %lu data bytes\n", env->hostname_dst, env->host_dst,
		sizeof(env->buf) - sizeof(*(env->icmp)));
}

void	pg_display_response(t_env *env, int bytes_receive, int seq,
		double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(env->res->ai_family, &(env->ip->ip_src.s_addr), ip, sizeof(ip));
	printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
	bytes_receive - sizeof(*(env->ip)), ip, seq, env->ip->ip_ttl, duration);
}

void	pg_display_stats(t_env *env)
{
	double	percentage_lost;
	double	avg;
	double	variant;

	percentage_lost = 0;
	avg = 0;
	variant = 0;
	if (env->packets_send != 0)
		percentage_lost = 100 - ((env->packets_receive * 100) /
								env->packets_send);
	printf("-- %s ping statistics ---\n", env->hostname_dst);
	printf("%u packets transmitted, %u packets received, %.2f%% packet loss\n",
		env->packets_send, env->packets_receive, percentage_lost);
	if (env->packets_send != 0)
		avg = env->cumul / env->packets_send;
	if ((env->packets_send - avg * avg) != 0)
		variant = env->cumul_s / env->packets_send - avg * avg;
	if (env->packets_receive > 0)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
			env->min, avg, env->max, sqrt(variant));
	exit(0);
}
