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

	while (1)
	{
		pg_configure_send(env);
		if ((nb_send = sendto(env->s, env->buf, sizeof(env->buf), 0,
			env->res->ai_addr, env->res->ai_addrlen)) < 0)
			ft_error_str_exit("Error sendto\n");
		pg_timer(1);
		pg_configure_receive(env);
		// if ((nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT)) < 0)
		// 	ft_error_str_exit("Error receive\n");
		nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT);
		printf("Recieve: %d\n", nb_receive);
	}
}
