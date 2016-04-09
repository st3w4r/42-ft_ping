/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:52:17 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/09 17:45:24 by ybarbier         ###   ########.fr       */
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

	while (tv_current.tv_sec < tv_next.tv_sec ||
			tv_current.tv_usec < tv_next.tv_usec)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			ft_error_str_exit("Error gettimeofday\n");
	}
}

static t_bool	pg_timeout(t_env *env)
{
	if (env->timeout_flag)
	{
		printf("Request timeout for icmp_seq %hu\n", env->seq);
		alarm(0);
		env->timeout_flag = FALSE;
		env->seq++;
		return (FALSE);
	}
	return (TRUE);
}

static t_bool	pg_loop_receive(t_env *env,
				struct timeval tv_start, struct timeval tv_end)
{
	double	duration;
	int		nb_receive;

	duration = 0;
	nb_receive = 0;
	pg_configure_receive(env);
	nb_receive = recvmsg(env->s, &(env->msg), MSG_DONTWAIT);
	gettimeofday(&tv_end, NULL);
	if (pg_timeout(env) == FALSE)
		return (FALSE);
	if (env->icmp->icmp_hun.ih_idseq.icd_id == env->pid)
	{
		duration = (((double)tv_end.tv_sec * 1000000.0 + tv_end.tv_usec) - \
		((double)tv_start.tv_sec * 1000000.0 + tv_start.tv_usec)) / 1000;
		env->packets_receive++;
		pg_duration_stats(env, duration);
		pg_display_response(env, nb_receive, env->seq, duration);
		pg_timer(env->interval);
		alarm(0);
		env->timeout_flag = FALSE;
		env->seq++;
		return (FALSE);
	}
	return (TRUE);
}

void	pg_loop(t_env *env)
{
	int				nb_send;
	struct timeval	tv_start;
	struct timeval	tv_end;

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
		alarm(env->timeout);
		while (pg_loop_receive(env, tv_start, tv_end));
	}
	pg_display_stats(env);
}
