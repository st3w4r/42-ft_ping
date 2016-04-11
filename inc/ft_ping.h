/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:51:48 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/11 14:50:46 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_H
# define PING_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <netdb.h>
# include <signal.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <float.h>
# include <math.h>

#define TRUE 1
#define FALSE 0

typedef unsigned char	t_bool;

enum	e_flags
{
	FLAGS_V = 1 << 0,
	FLAGS_H = 1 << 1
};

typedef struct	s_env {
	int						flags;
	unsigned int			count;
	unsigned int			interval;
	unsigned int			timeout;
	t_bool					timeout_flag;
	pid_t					pid;
	unsigned int			packets_send;
	unsigned int			packets_receive;
	unsigned short			seq;
	double					min;
	double					max;
	double					cumul;
	double					cumul_s;
	// SOCKET
	int						s;
	struct addrinfo			hints;
	struct addrinfo			*res;
	// SEND
	char					buf[76];
	char					*host_src;
	char					*host_dst;
	char					*hostname_dst;
	struct ip				*ip;
	struct icmp				*icmp;
	// RECIEVE
	struct iovec			iov[1];
	struct msghdr			msg;
	char					bufControl[1000];
}							t_env;

t_env	env;

/*
** Name: pg_connect
** Desc: Configure and open socket
*/

int		pg_open_socket(t_env *env);
void	pg_configure_header(t_env *env);
void	pg_configure_send(t_env *env, unsigned short id, unsigned short seq);
void	pg_configure_receive(t_env *env);

/*
** Name: pg_loop
** Desc: Loop to send ICMP ECHO and receive ICMP REPLY
*/

void	pg_loop(t_env *env);

/*
** Name: pg_display
** Desc: Functions to display informations
*/
void	pg_display_info(t_env *env);
void	pg_display_response(t_env *env, int bytes_receive, int seq,
		double duration);
void	pg_display_stats(t_env *env);

/*
** Name: pg_helper
** Desc: Helpers Functions
*/

//unsigned short	pg_icmp_checksum(char type, char code, unsigned short id,
//	unsigned short seq);
unsigned short	pg_icmp_checksum(unsigned short *buf, int len);
void	pg_sig_handler(int sig);
char	*pg_get_ip_from_hostname(char *hostname);
void	pg_duration_stats(t_env *env, double duration);

#endif
