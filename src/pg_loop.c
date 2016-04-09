/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:52:17 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/09 16:22:31 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
/*
void	pg_display_info(t_env *env)
{
//	char ip[INET_ADDRSTRLEN];

//	inet_ntop(env->res->ai_family, &(env->ip->ip_dst.s_addr), ip, sizeof(ip));
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
		percentage_lost = 100 - ((env->packets_receive * 100) / env->packets_send);
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
*/

void	pg_timeout(t_env *env)
{
//	printf("Request timeout for icmp_seq %hu\n", env->seq);
//	env->seq++;
//	env->timeout_flag = TRUE;
}

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

static int	pg_loop_receive(t_env *env, double duration,
		struct timeval tv_start, struct timeval tv_end, int nb_receive)
{
	if ((env->timeout_flag))
	{
		alarm(env->timeout);
		env->timeout_flag = FALSE;
	}
	pg_configure_receive(env);
	nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT);
	duration = 0;
	gettimeofday(&tv_end, NULL);

	if (env->timeout_flag)
	{
		printf("Request timeout for icmp_seq %hu\n", env->seq);
		alarm(0);
		env->timeout_flag = TRUE;
		env->seq++;
		return (FALSE);
	}
	
	if (env->icmp->icmp_hun.ih_idseq.icd_id == env->pid)
	{
		duration = (((double)tv_end.tv_sec * 1000000.0 + tv_end.tv_usec) - \
		((double)tv_start.tv_sec * 1000000.0 + tv_start.tv_usec)) / 1000;
		env->packets_receive++;
		pg_duration_stats(env, duration);
		pg_display_response(env, nb_receive, env->seq, duration);
		pg_timer(env->interval);
		alarm(0);
		env->timeout_flag = TRUE;
		env->seq++;
		return (FALSE);
	}
	return (TRUE);
}

void	pg_loop(t_env *env)
{
	int				nb_send;
	int				nb_receive;
	struct timeval	tv_start;
	struct timeval	tv_end;
	double			duration;

	env->packets_send = 0;
	env->packets_receive = 0;
	env->seq = 0;
	pg_display_info(env);
	while (env->packets_send < env->count)
	{
		pg_configure_send(env, env->pid, env->seq);
		gettimeofday(&tv_start, NULL);
		if ((nb_send = sendto(env->s, env->buf, sizeof(env->buf), 0,
			env->res->ai_addr, env->res->ai_addrlen)) < 0)
			ft_error_str_exit("Error sendto\n");
		if (nb_send >= 0)
			env->packets_send++;
		env->timeout_flag = TRUE;
		while (pg_loop_receive(env, duration, tv_start, tv_end, nb_receive));
	}
	pg_display_stats(env);
}
