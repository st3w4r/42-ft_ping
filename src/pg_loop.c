/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:52:17 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/04 19:30:51 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	pg_display_response(t_env *env, int bytes_receive, int seq,
		double duration)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(env->res->ai_family, &(env->ip->ip_src.s_addr), ip, sizeof(ip));
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n",
	bytes_receive, ip, seq, env->ip->ip_ttl, duration);
}

static void	pg_timer(int interval)
{
	struct timeval tv_current;
	struct timeval tv_next;

	if (gettimeofday(&tv_current, NULL) < 0)
		ft_error_str_exit("Error gettimeofday\n");
	tv_next = tv_current;
	tv_next.tv_sec += interval;

	while (tv_current.tv_sec < tv_next.tv_sec)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			ft_error_str_exit("Error gettimeofday\n");
	}
//	printf("Time: %ld %ld\n", tv_current.tv_sec, tv_current.tv_usec);
}

void	pg_loop(t_env *env)
{
	int			nb_send;
	int			nb_receive;
	int			packets_send;
	int			packets_receive;
	unsigned short	seq;
	struct timeval	tv_start;
	struct timeval	tv_end;
	double	duration;

	packets_send = 0;
	packets_receive = 0;
	seq = 0;
	while (packets_send < 10)
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
			packets_send++;
		}

//		pg_configure_receive(env);

		while (1)
		{
			pg_configure_receive(env);
			nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT);
			duration = 0;
			gettimeofday(&tv_end, NULL);
			if ((tv_end.tv_sec - tv_start.tv_sec) >= env->timeout)
			{
				printf("Request timeout for icmp_seq %hu\n", seq);
				seq++;
				break;
			}
			if (env->icmp->icmp_hun.ih_idseq.icd_id == env->pid)
			{
				duration = (((double)tv_end.tv_sec * 1000000.0 + tv_end.tv_usec) - \
				((double)tv_start.tv_sec * 1000000.0 + tv_start.tv_usec)) / 1000;
				packets_receive++;
				
				pg_display_response(env, nb_receive, seq, duration);
				printf("Recieve: %d\n", nb_receive);
				pg_timer(1);
				seq++;
				break;
			}
		}
	}
	printf("%d packets transmitted, %d packets received\n", packets_send, packets_receive);
}
