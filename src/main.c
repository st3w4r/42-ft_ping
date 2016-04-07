/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:22:32 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/07 19:19:08 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	pg_error_usage(void)
{
	ft_error_str_exit("Usage: ping [OPTIONS] HOST\n");
}

static void	pg_parse_flags_one_arg(t_env *env, char *str)
{
	if (ft_strchr(str, 'v') != NULL)
		env->flags |= FLAGS_V;
	if (ft_strchr(str, 'h') != NULL)
		env->flags |= FLAGS_H;
}

static int	pg_parse_flags(t_env *env, int nb_args, char **args)
{
	int i;

	i = 1;
	while (i < nb_args && ft_strncmp("-", args[i], 1) == 0)
	{
		if (args[i][1] && ft_isdigit(args[i][1]) == 1)
			break ;
		pg_parse_flags_one_arg(env, args[i]);
		++i;
	}
	return (i);
}

int			main(int argc, char **argv)
{
	int		pos_args;

	if (argc <= 1)
		pg_error_usage();
	env.flags = 0;
	pos_args = pg_parse_flags(&env, argc, argv);
	env.hostname_dst = argv[pos_args];
	env.host_dst = pg_get_ip_from_hostname(argv[pos_args]);
	env.host_src = "0.0.0.0";
	env.interval = 1;
	env.timeout = 1;
	env.count = 10;
	env.min = DBL_MAX;
	env.pid = getpid();
	pg_open_socket(&env);
	pg_configure_header(&env);
	signal(SIGALRM, pg_sig_handler);
	signal(SIGINT, pg_sig_handler);
	pg_loop(&env);
	return (0);
}
