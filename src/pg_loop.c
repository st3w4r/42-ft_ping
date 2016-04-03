/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:52:17 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/31 18:44:28 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	pg_display_response(t_env *env, int bytes_receive, int seq)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(env->res->ai_family, &(env->ip->ip_src.s_addr), ip, sizeof(ip));
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time= ms\n",
	bytes_receive, ip, seq, env->ip->ip_ttl);
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
	printf("Time: %ld %ld\n", tv_current.tv_sec, tv_current.tv_usec);
}

void	pg_loop(t_env *env)
{
	int			nb_send;
	int			nb_receive;
	int			packets_send;
	int			packets_receive;
	unsigned short	seq;

	packets_send = 0;
	packets_receive = 0;
	seq = 0;
	while (packets_send < 10)
	{
		pg_configure_send(env, 6921, seq);

		if ((nb_send = sendto(env->s, env->buf, sizeof(env->buf), 0,
			env->res->ai_addr, env->res->ai_addrlen)) < 0)
			ft_error_str_exit("Error sendto\n");
		if (nb_send >= 0)
			packets_send++;

		pg_configure_receive(env);

		nb_receive = recvmsg(env->s, &(env->msg), 0);
		if (nb_receive >= 0)
			packets_receive++;

		pg_display_response(env, nb_receive, seq);
		seq++;
		printf("Recieve: %d\n", nb_receive);
		pg_timer(1);
	}
	printf("%d packets transmitted, %d packets received\n", packets_send, packets_receive);
}
