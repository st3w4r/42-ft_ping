/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:51:48 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/31 17:31:06 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_H
# define PING_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>

enum	e_flags
{
	FLAGS_V = 1 << 0,
	FLAGS_H = 1 << 1
};

typedef struct	s_env {
	int		flags;
	int		s;
	struct addrinfo	hints;
	struct addrinfo	*res;
	char	buf[64];
	char	*host_src;
	char	*host_dst;
}				t_env;

/*
** Name: pg_connect
** Desc: Configure and open socket
*/

int		pg_open_socket(t_env *env);
void	pg_configure_header(t_env *env);

/*
** Name: pg_loop
** Desc: Loop to send ICMP ECHO and receive ICMP REPLY
*/



#endif
