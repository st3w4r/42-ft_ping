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

void	pg_loop(t_env *env)
{
	int nb_send;

	while (1)
	{
		if ((nb_send = sendto(env->s, env->buf, sizeof(env->buf), 0, 
			env->res->ai_addr, env->res->ai_addrlen)) < 0)
			ft_error_str_exit("Error sendto");
	}
}
