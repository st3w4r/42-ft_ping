/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/04 15:16:39 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/07 19:05:15 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

unsigned short	pg_icmp_checksum(char type, char code, unsigned short id,
		unsigned short seq)
{
	unsigned short sum;

	sum = 0;
	sum = type << 8;
	sum += code;
	sum += id;
	sum += seq;
	return (~(sum));
}

void pg_sig_handler(int sig)
{
	if (sig == SIGINT)
		pg_display_stats(&env);
	if (sig == SIGALRM)
		pg_timeout(&env);
}

void	pg_timeout(t_env *env)
{
	env->timeout_flag = TRUE;
}

char	*pg_get_ip_from_hostname(char *hostname)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct sockaddr_in *sa_in;
	char *ip_share;

	ft_memset(&(hints), 0, sizeof(hints));
	hints.ai_family = AF_INET;

	if((ip_share = malloc(INET_ADDRSTRLEN)) < 0)
		ft_error_str_exit("Error malloc");
	if (getaddrinfo(hostname, NULL, &hints, &(res)) < 0)
		ft_error_str_exit("Error getaddrinfo\n");
	sa_in = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(res->ai_family, &(sa_in->sin_addr), ip_share, INET_ADDRSTRLEN);
	return (ip_share);
}

void	pg_duration_stats(t_env *env, double duration)
{
	if (duration > env->max)
		env->max = duration;
	if (duration < env->min)
		env->min = duration;
	env->cumul += duration;
}
