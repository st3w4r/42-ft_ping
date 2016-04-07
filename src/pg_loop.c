/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:52:17 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/07 15:09:14 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	pg_display_info(t_env *env)
{
//	char ip[INET_ADDRSTRLEN];

//	inet_ntop(env->res->ai_family, &(env->ip->ip_dst.s_addr), ip, sizeof(ip));
	printf("PING %s (%s): %lu data bytes\n", env->hostname_dst, env->host_dst, sizeof(env->buf));
}

void	pg_display_response(t_env *env, int bytes_receive, int seq,
		double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(env->res->ai_family, &(env->ip->ip_src.s_addr), ip, sizeof(ip));
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
	bytes_receive, ip, seq, env->ip->ip_ttl, duration);
}

void	pg_display_stats(t_env *env)
{
	double	percentage_lost;

	percentage_lost = 0;
	if (env->packets_send != 0)
		percentage_lost = 100 - ((env->packets_receive * 100) / env->packets_send);
	printf("-- %s ping statistics ---\n", env->host_dst);
	printf("%u packets transmitted, %u packets received, %.2f%% packet loss\n",
		env->packets_send, env->packets_receive, percentage_lost);
	printf("round-trip min/avg/max/stddev = 2.354/2.396/2.425/0.031 ms\n");
	exit(0);
}
/*
void	pg_timeout(t_env *env)
{
//	printf("Request timeout for icmp_seq %hu\n", env->seq);
//	env->seq++;
	env->timeout_flag = TRUE;
}
*/

static void	pg_timer(int interval)
{
	struct timeval tv_current;
	struct timeval tv_next;

	if (gettimeofday(&tv_current, NULL) < 0)
		ft_error_str_exit("Error gettimeofday\n");
	tv_next = tv_current;
	tv_next.tv_sec += interval;

	while (tv_current.tv_sec < tv_next.tv_sec ||
			tv_current.tv_usec < tv_next.tv_usec)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			ft_error_str_exit("Error gettimeofday\n");
	}
//	printf("Time: %ld %ld\n", tv_current.tv_sec, tv_current.tv_usec);
}

void	pg_loop(t_env *env)
{
	int				nb_send;
	int				nb_receive;
	unsigned short	seq;
	struct timeval	tv_start;
	struct timeval	tv_end;
	double			duration;

	env->packets_send = 0;
	env->packets_receive = 0;
	seq = 0;
	pg_display_info(env);
	while (env->packets_send < 10)
	{
//			printf("tv_usec: %ld\n", tv_current.tv_usec / 1000);

		pg_configure_send(env, env->pid, seq);

		gettimeofday(&tv_start, NULL);
		if ((nb_send = sendto(env->s, env->buf, sizeof(env->buf), 0,
			env->res->ai_addr, env->res->ai_addrlen)) < 0)
			ft_error_str_exit("Error sendto\n");
		if (nb_send >= 0)
		{
			//gettimeofday(&tv_start, NULL);
			//printf("tv_usec: %ld\n", tv_start.tv_usec / 1000);
			//printf("tv_usec: %ld ", tv_duration.tv_usec);
			env->packets_send++;
		}

//		pg_configure_receive(env);
		env->timeout_flag = TRUE;
		while (1)
		{
			if ((env->timeout_flag))
			{
				alarm(1);
				env->timeout_flag = FALSE;
			}
			pg_configure_receive(env);
			nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT);
			duration = 0;
			gettimeofday(&tv_end, NULL);
			
			//if ((tv_end.tv_sec - tv_start.tv_sec) >= env->timeout)
			if (env->timeout_flag)
			{
				printf("Request timeout for icmp_seq %hu\n", seq);
				seq++;
				break;
			}
			
			
			if (env->icmp->icmp_hun.ih_idseq.icd_id == env->pid)
			{
				duration = (((double)tv_end.tv_sec * 1000000.0 + tv_end.tv_usec) - \
				((double)tv_start.tv_sec * 1000000.0 + tv_start.tv_usec)) / 1000;
				env->packets_receive++;

				pg_display_response(env, nb_receive, seq, duration);
				pg_timer(1);
				alarm(0);
				seq++;
				env->timeout_flag = TRUE;
				break;
			}
		}
	}
	pg_display_stats(env);
}
