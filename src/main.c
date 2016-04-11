/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:22:32 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/11 15:32:17 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	pg_error_usage(void)
{
	ft_error_str_exit("Usage: ft_ping [-hvDciWm] HOST\n");
}

static void	pg_help(void)
{
	printf("Usage: ping [OPTION...] HOST ... \n"
"Send ICMP ECHO_REQUEST packets to network hosts.\n"
"\n"
"   -v                verbose output\n"
"   -D                set the DONT FRAGMENT flag\n"
"   -c=NUMBER         stop after sending NUMBER packets\n"
"   -i=NUMBER         wait NUMBER seconds between sending each packet\n"
"   -W=NUMBER         number of seconds to wait for response\n"
"   -m=NUMBER          set the ttl\n"
"\n"
"   -h                give this help list\n");
	exit(0);
}

static void	pg_parse_flags_one_arg(t_env *env, char *str)
{
	if (ft_strchr(str, 'v') != NULL)
		env->flags |= FLAGS_V;
	if (ft_strchr(str, 'h') != NULL)
		pg_help();
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

int			pg_options(t_env *env, int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "vhDc:i:W:m:")) != -1)
	{
		if (opt == 'h')
			pg_help();
		else if (opt == 'v')
			env->flags |= FLAGS_V;
		else if (opt == 'D')
			env->df_flag = TRUE;
		else if (opt == 'c')
		{
			if (ft_atoi(optarg) >= 0)
				env->count = ft_atoi(optarg);
			else
				ft_error_str_exit("ft_ping: invalid value\n");
		}
		else if (opt == 'i')
		{
			if (ft_atoi(optarg) >= 0)
				env->interval = ft_atoi(optarg);
			else
				ft_error_str_exit("ft_ping: invalid value\n");
		}
		else if (opt == 'W')
		{
			if (ft_atoi(optarg) > 0)
				env->timeout = ft_atoi(optarg);
			else
				ft_error_str_exit("ft_ping: invalid value\n");
		}
		else if (opt == 'm')
		{
			if (ft_atoi(optarg) >= 0 && ft_atoi(optarg) <= 255)
				env->ttl = ft_atoi(optarg);
			else
				ft_error_str_exit("ft_ping: invalid value\n");
		}
		else
			pg_error_usage();
	}
	return (optind);
}

int			main(int argc, char **argv)
{
	int		pos_args;

	if (argc <= 1)
		pg_error_usage();
	if (getuid() != 0)
		ft_error_str_exit("ft_ping: Operation not permitted\n");
	env.flags = 0;
	env.count = 0;
	env.interval = 1;
	env.timeout = 1;
	env.ttl = 64;
	pos_args = pg_options(&env, argc, argv);
//	pos_args = pg_parse_flags(&env, argc, argv);
	env.hostname_dst = argv[pos_args];
	env.host_dst = pg_get_ip_from_hostname(argv[pos_args]);
	env.host_src = "0.0.0.0";
	env.min = DBL_MAX;
	env.pid = getpid();
	pg_open_socket(&env);
	pg_configure_header(&env);
	signal(SIGALRM, pg_sig_handler);
	signal(SIGINT, pg_sig_handler);
	pg_loop(&env);
	return (0);
}
