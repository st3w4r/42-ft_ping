/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 16:22:32 by ybarbier          #+#    #+#             */
/*   Updated: 2016/04/09 17:23:56 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	pg_error_usage(void)
{
	ft_error_str_exit("Usage: ping [OPTIONS] HOST\n");
}

static void	pg_help(void)
{
	printf("Usage: ping [OPTION...] HOST ... \n"
"Send ICMP ECHO_REQUEST packets to network hosts.\n"
"\n"
"Options controlling ICMP request types:\n"
"   --address              send ICMP_ADDRESS packets (root only)\n"
"   --echo                 send ICMP_ECHO packets (default)\n"
"   --timestamp            send ICMP_TIMESTAMP packets\n"
"   -t, --type=TYPE            send TYPE packets\n"
"\n"
"Options valid for all request types:\n"
"\n"
"   -c, --count=NUMBER         stop after sending NUMBER packets\n"
"   -d, --debug                set the SO_DEBUG option\n"
"   -i, --interval=NUMBER      wait NUMBER seconds between sending each packet\n"
"   -n, --numeric              do not resolve host addresses\n"
"   -r, --ignore-routing       send directly to a host on an attached network\n"
"   -v, --verbose              verbose output\n"
"   -w, --timeout=N            stop after N seconds\n"
"   -W, --linger=N             number of seconds to wait for response\n"
"\n"
"Options valid for --echo requests:\n"
"\n"
"   -f, --flood                flood ping (root only)\n"
"   -l, --preload=NUMBER       send NUMBER packets as fast as possible before\n"
"   falling into normal mode of behavior (root only)\n"
"   -p, --pattern=PATTERN      fill ICMP packet with given pattern (hex)\n"
"   -q, --quiet                quiet output\n"
"   -R, --route                record route\n"
"   -s, --size=NUMBER          send NUMBER data octets\n"
"\n"
"   -?, --help                 give this help list\n"
"   --usage                give a short usage message\n"
"   -V, --version              print program version\n"
"\n"
"Mandatory or optional arguments to long options are also mandatory or optional"
"for any corresponding short options.\n"
"\n"
"Options marked with (root only) are available only to superuser.\n");
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
	env.count = 20;
	env.min = DBL_MAX;
	env.pid = getpid();
	pg_open_socket(&env);
	pg_configure_header(&env);
	signal(SIGALRM, pg_sig_handler);
	signal(SIGINT, pg_sig_handler);
	pg_loop(&env);
	return (0);
}
