/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:02:04 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 19:22:30 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <ft_fc_str.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void		init_socks(t_server *srv, fd_set *rdfs)
{
	int			i;

	i = -1;
	FD_ZERO(rdfs);
	FD_SET(STDIN_FILENO, rdfs);
	FD_SET(srv->sock, rdfs);
	while (++i < srv->actual)
		FD_SET(srv->cl[i]->csock, rdfs);
	if (select(srv->max + 1, rdfs, NULL, NULL, NULL) == -1)
		ft_exit("Select Error\n");
}

static void		add_client(t_server *srv, fd_set *rdfs)
{
	srv->cl[srv->actual] = new_client(srv);
	if (srv->cl[srv->actual]->csock > srv->max)
		srv->max = srv->cl[srv->actual]->csock;
	FD_SET(srv->cl[srv->actual]->csock, rdfs);
	printf("%s", DYELLOW);
	printf("%s connected.", srv->cl[srv->actual]->nickname);
	printf("%s\n", NONE);
	welcome(srv, srv->actual);
	srv->actual++;
}

static void		client_loop(t_server *srv, fd_set *rdfs)
{
	int			i;
	char		*msg;

	i = -1;
	while (++i < srv->actual)
	{
		if (FD_ISSET(srv->cl[i]->csock, rdfs))
		{
			msg = get_message(srv->cl[i]->csock);
			if (!msg || !ft_strcmp(msg, "/quit") || !ft_strcmp(msg, "/exit"))
			{
				if (msg)
					free(msg);
				send_client(srv->cl[i]->csock, "\033[1A\r\033[K", NULL);
				kill_client(srv, i);
			}
			else
				print_message(srv, i, &msg);
		}
	}
}

void			be_server(t_server *srv)
{
	fd_set		rdfs;

	while (42)
	{
		init_socks(srv, &rdfs);
		if (FD_ISSET(STDIN_FILENO, &rdfs))
			break ;
		else if (FD_ISSET(srv->sock, &rdfs))
			add_client(srv, &rdfs);
		else
			client_loop(srv, &rdfs);
	}
}

int				main(int ac, char **av)
{
	t_server	*server;
	int			max_clients;

	max_clients = check_validity(ac, av);
	server = init_server(av[1]);
	server->max_clients = max_clients;
	be_server(server);
	close(server->sock);
	kill_all_clients(server);
	free(server->cl);
	free(server);
	return (0);
}
