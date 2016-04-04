/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:22:32 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/04 15:49:53 by ybarbier         ###   ########.fr       */
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
	t_env	env;
	int		pos_args;

	if (argc <= 1)
		pg_error_usage();
	env.flags = 0;
	pos_args = pg_parse_flags(&env, argc, argv);
	env.host_dst = argv[1];
	env.host_src = argv[2];
	env.interval = 1;
	env.pid = getpid();
	printf("Pid: %d\n", env.pid);
	pg_open_socket(&env);
	pg_configure_header(&env);
	pg_loop(&env);
	return (0);
}
