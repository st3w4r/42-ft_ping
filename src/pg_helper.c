/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pg_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/04 15:16:39 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/05 16:35:36 by ybarbier         ###   ########.fr       */
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
