/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:02:04 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 09:06:23 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ft_fc_conversion.h>
#include <ft_fc_print.h>
#include <ft_fc_str.h>
#include <stdlib.h>
#include <stdio.h>
#include <colors.h>

int			check_validity(int ac, char **av)
{
	int		port;
	int		max_clients;

	if (ac < 2)
		ft_exit("\033[1;33mUsage : ./server <port> [max_clients]\033[0m\n");
	port = ft_atoi(av[1]);
	max_clients = (ac >= 3 ? ft_atoi(av[2]) : MAX_CLIENTS);
	max_clients = (max_clients <= 0 ? MAX_CLIENTS : max_clients);
	if (port <= 0)
		ft_exit("\033[1;33mInvalid Port\033[0m\n");
	return (max_clients);
}

void				new_client(t_server *srv, int actual)
{
	t_clients		*client;
	unsigned int	size;
	static int		nb = 0;
	char			*nb_char;

	client = &srv->clients[actual];
	size = sizeof(client->csin);
	client->csock = accept(srv->sock, (t_sockaddr *)&client->csin, &size);
	ft_assert(client->csock != -1, "Accept Error\n");
	nb_char = ft_itoa(nb++);
	srv->clients[actual].nickname = ft_strjoin("Visitor", nb_char);
	free(nb_char);
	printf("%s - %p\n", srv->clients[actual].nickname, srv->clients[actual].nickname);
}

t_server			*init_server(char *port)
{
	t_server		*serv;

	serv = (t_server *)malloc(sizeof(t_server));
	ft_assert(serv != NULL, "Can't create server\n");
	serv->sock = socket(AF_INET, SOCK_STREAM, 0);
	ft_assert(serv->sock != -1, "Socket Error\n");
	serv->sin.sin_family = AF_INET;
	serv->sin.sin_addr.s_addr = INADDR_ANY;
	serv->sin.sin_port = htons(ft_atoi(port));
	if (bind(serv->sock, (t_sockaddr *)&serv->sin, sizeof(serv->sin)) == -1)
		ft_exit("Bind Error\n");
	if (listen(serv->sock, serv->max_clients) == -1)
		ft_exit("Listen Problem\n");
	serv->clients = (t_clients *)malloc(serv->max_clients * sizeof(t_clients));
	serv->max = serv->sock;
	serv->actual = 0;
	return (serv);
}

void		send_all_but_one(t_server *srv, int one, char *message, char *color)
{
	int		i;

	i = -1;
	while (++i < srv->actual)
	{
		if (i != one)
			send_client(srv->clients[i].csock, message, color);
	}
}

void		change_nickname(t_server *srv, char *nickname, int i)
{
	int		size;

	size = ft_strlen(nickname);
	if (size <= 0 || size > 9)
	{
		send_client(srv->clients[i].csock,
				"Nickname must be between 1 and 9 char long.\n", DRED);
		return ;
	}
	printf("%s%s %s%s ", DYELLOW, "Client", DRED, srv->clients[i].nickname);
	printf("%s%s %s%s", DYELLOW, "changed nickname to", DRED, nickname);
	printf("%s.\n", NONE);
	send_all_but_one(srv, i, "Client ", DYELLOW);
	send_all_but_one(srv, i, srv->clients[i].nickname, DRED);
	send_all_but_one(srv, i, " changed nickname to ", DYELLOW);
	send_all_but_one(srv, i, nickname, DRED);
	send_all_but_one(srv, i, ".", DYELLOW);
	send_client(srv->clients[i].csock, "Succefully changed Nickname.\n", DYELLOW);
	if (srv->clients[i].nickname)
		free(srv->clients[i].nickname);
	srv->clients[i].nickname = ft_strdup(nickname);
}
void		srv_cmds(t_server *srv, char *cmd, int i)
{
	if (!ft_strncmp(cmd, "/nickname ", 9))
		change_nickname(srv, &cmd[10], i);
	else if (!ft_strncmp(cmd, "/nick ", 5))
		change_nickname(srv, &cmd[6], i);
	else if (!ft_strncmp(cmd, "/n ", 2))
		change_nickname(srv, &cmd[3], i);
	else
		send_client(srv->clients[i].csock, "Bad command.\n", DRED);
}

void			kill_client(t_server *srv, int i)
{
	t_clients	*src;
	t_clients	*dest;
	size_t		size;

	src = &srv->clients[i];
	dest = &srv->clients[i + 1];
	size = (srv->actual - i - 1) * sizeof(t_clients);
	printf("%sClient %s disconnected.%s\n", DRED, srv->clients[i].nickname, NONE);
	close(srv->clients[i].csock);
	free(srv->clients[i].nickname);
	memmove(src, dest, size);
	srv->actual--;
}

void			print_message(t_server *srv, int i, char **message)
{
	int			j;

	send_client(srv->clients[i].csock, "\033[1A\r\033[K", NULL);
	if (!**message)
	{
		free(*message);
		return ;
	}
	if ((*message)[0] == '/')
	{
		srv_cmds(srv, *message, i);
		free(*message);
		return ;
	}
	printf("A\n");
	printf("%s : %s\n", srv->clients[i].nickname, *message);
	printf("B\n");
	j = -1;
	while (++j < srv->actual)
	{
		send_client(srv->clients[j].csock, srv->clients[i].nickname, DYELLOW);
		send_client(srv->clients[j].csock, " : ", NULL);
		send_client(srv->clients[j].csock, *message, NULL);
		send_client(srv->clients[j].csock, "\n", NULL);
	}
	free(*message);
}

void			be_server(t_server *srv)
{
	char		*message;
	fd_set		rdfs;
	int			i;

	while (42)
	{
		i = -1;
		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO, &rdfs);
		FD_SET(srv->sock, &rdfs);
		while (++i < srv->actual)
			FD_SET(srv->clients[i].csock, &rdfs);
		if (select(srv->max + 1, &rdfs, NULL, NULL, NULL) == -1)
			ft_exit("Select Error\n");
		if (FD_ISSET(STDIN_FILENO, &rdfs))
			break ;
		else if (FD_ISSET(srv->sock, &rdfs))
		{
			new_client(srv, srv->actual);
			if (srv->clients[srv->actual].csock > srv->max)
				srv->max = srv->clients[srv->actual].csock;
			FD_SET(srv->clients[srv->actual].csock, &rdfs);
			printf("%s", DYELLOW);
			printf("%s connected.", srv->clients[srv->actual].nickname);
			printf("%s\n", NONE);
			send_all_but_one(srv, srv->actual,
							srv->clients[srv->actual].nickname, DYELLOW);
			send_all_but_one(srv, srv->actual, " connected.\n", DYELLOW);
			srv->actual++;
		}
		else
		{
			i = -1;
			while (++i < srv->actual)
			{
				if (FD_ISSET(srv->clients[i].csock, &rdfs))
				{
					message = get_message(srv->clients[i].csock);
					printf("O\n");
					printf("%s\n", srv->clients[i].nickname);
					if (!message)
						kill_client(srv, i);
					else
						print_message(srv, i, &message);
				}
			}
		}
	}
}

int		main(int ac, char **av)
{
	t_server	*server;
	int			max_clients;

	max_clients = check_validity(ac, av);
	server = init_server(av[1]);
	server->max_clients = max_clients;
	be_server(server);
	if (server->clients->csock == 0)
		ft_exit("Wrote on server\n");

	close(server->sock);
/*	close(server->clients->csock);
	free(server->clients->nickname);*/
	free(server->clients);
	free(server);
	return (0);
}
