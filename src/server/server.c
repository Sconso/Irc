/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:02:04 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 15:05:10 by Myrkskog         ###   ########.fr       */
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

void		print_chan(t_server *srv, char *chan, int cl)
{
	int		nb;
	int		i;
	char	*nb_char;

	if (chan == 0)
		return ;
	nb = 0;
	i = -1;
	while (++i < srv->actual)
		nb += (srv->clients[i]->chan == chan ? 1 : 0);
	nb_char = ft_itoa(nb);
	send_client(cl, "\t", DGREEN);
	send_client(cl, chan, DGREEN);
	send_client(cl, " : ", NULL);
	send_client(cl, nb_char, NULL);
	send_client(cl, " Connected now.\n", NULL);
	free(nb_char);
}

void		welcome(t_server *srv, int active)
{
	int		cl;

	cl = srv->clients[active]->csock;
	send_client(cl, "\tWelcome to SC_IRC ", DGREEN);
	send_client(cl, srv->clients[active]->nickname, DGREEN);
	send_client(cl, "\n\nBe polite, respectfull and don't panic.\n\n", NULL);
	send_client(cl, "COMMANDS :\n", DRED);
	send_client(cl, "\t/n, /nick, /nickname <name> : ", DGREEN);
	send_client(cl, "Select a new nickname.\n", NULL);
	send_client(cl, "\t/join <chan> : ", DGREEN);
	send_client(cl, "Join a chan.\n", NULL);
	send_client(cl, "\t/leave : ", DGREEN);
	send_client(cl, "Leave the current chan.\n", NULL);
	send_client(cl, "\t/who : ", DGREEN);
	send_client(cl, "Show who is on this chan.\n", NULL);
	send_client(cl, "\t/me <message> : ", DGREEN);
	send_client(cl, "Send a roleplay formated message\n", NULL);
	send_client(cl, "\t/exit, /quit : ", DGREEN);
	send_client(cl, "Leave the client.\n", NULL);
	send_client(cl, "\t/help : ", DGREEN);
	send_client(cl, "Print this message.\n", NULL);
	send_client(cl, "\nActive Chans :\n", DRED);
	print_chan(srv, MUSIC, cl);
	print_chan(srv, CODING, cl);
	print_chan(srv, GAMES, cl);
	send_client(cl, "\nAnd now... Have fun !\n", NULL);
}

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

t_clients			*new_client(t_server *srv)
{
	t_clients		*client;
	unsigned int	size;
	static int		nb = 1;
	char			*nb_char;

	client = (t_clients *)malloc(sizeof(t_clients));
	ft_assert(client != NULL, "Can't create server\n");
	size = sizeof(client->csin);
	client->csock = accept(srv->sock, (t_sockaddr *)&client->csin, &size);
	ft_assert(client->csock != -1, "Accept Error\n");
	nb_char = ft_itoa(nb++);
	client->nickname = ft_strjoin("Visitor", nb_char);
	client->chan = NO_CHAN;
	free(nb_char);
	return (client);
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
	serv->clients = (t_clients **)malloc(serv->max_clients * sizeof(t_clients *));
	serv->max = serv->sock;
	serv->actual = 0;
	return (serv);
}

void		send_all_but_one(t_server *srv, int one, char *message, char *color)
{
	int		i;
	int		sock;

	i = -1;
	while (++i < srv->actual)
	{
		sock = srv->clients[i]->csock;
		if (srv->clients[i]->chan != srv->clients[one]->chan)
			continue ;
		if (i != one)
			send_client(sock, message, color);
	}
}

void		change_nickname(t_server *srv, char *nickname, int i)
{
	int		size;

	size = ft_strlen(nickname);
	if (size <= 0 || size > 9)
	{
		send_client(srv->clients[i]->csock,
				"Nickname must be between 1 and 9 char long.\n", DRED);
		return ;
	}
	printf("%s%s %s%s ", DYELLOW, "Client", DRED, srv->clients[i]->nickname);
	printf("%s%s %s%s", DYELLOW, "changed nickname to", DRED, nickname);
	printf("%s.\n", NONE);
	send_all_but_one(srv, i, "Client ", DYELLOW);
	send_all_but_one(srv, i, srv->clients[i]->nickname, DRED);
	send_all_but_one(srv, i, " changed nickname to ", DYELLOW);
	send_all_but_one(srv, i, nickname, DRED);
	send_all_but_one(srv, i, ".\n", DYELLOW);
	send_client(srv->clients[i]->csock, "Successfully changed Nickname.\n", DYELLOW);
	if (srv->clients[i]->nickname)
		free(srv->clients[i]->nickname);
	srv->clients[i]->nickname = ft_strdup(nickname);
}

void		leave_chan(t_server *srv, int active, char warn)
{
	int		sock;

	sock = srv->clients[active]->csock;
	if (srv->clients[active]->chan == NO_CHAN)
	{
		if (warn)
			send_client(sock, "You aren't on a chan.\n", DRED);
		return ;
	}
	send_client(sock, "You left ", DYELLOW);
	send_client(sock, srv->clients[active]->chan, DYELLOW);
	send_client(sock, "\n", NONE);
	send_all_but_one(srv, active, srv->clients[active]->nickname, DRED);
	send_all_but_one(srv, active, " left the chan\n", DRED);
	printf("%s%s left ", DYELLOW, srv->clients[active]->nickname);
	printf("%s%s\n", srv->clients[active]->chan, NONE);
	srv->clients[active]->chan = NO_CHAN;
}

void		join_chan(t_server *srv, int cl, char *chan)
{
	int		sock;

	leave_chan(srv, cl, 0);
	sock = srv->clients[cl]->csock;
	if (!ft_strcmp(chan, MUSIC))
		srv->clients[cl]->chan = MUSIC;
	else if (!ft_strcmp(chan, CODING))
		srv->clients[cl]->chan = CODING;
	else if (!ft_strcmp(chan, GAMES))
		srv->clients[cl]->chan = GAMES;
	else
	{
		send_client(sock, "Not a valid chan.\n", DRED);
		send_client(sock, "Type /help to see the chans.\n", DRED);
		return ;
	}
	send_client(sock, "\n\tWelcome to ", DYELLOW);
	send_client(sock, chan, DYELLOW);
	send_client(sock, ", have fun !\n\n", DYELLOW);
	send_all_but_one(srv, cl, srv->clients[cl]->nickname, DYELLOW);
	send_all_but_one(srv, cl, " connected.\n", DYELLOW);
	printf("%s%s joined ", DYELLOW, srv->clients[cl]->nickname);
	printf("%s%s\n", chan, NONE);
}

void		who_cmd(t_server *srv, int active)
{
	int		sock;
	int		i;

	sock = srv->clients[active]->csock;
	i = -1;
	send_client(sock, "\n\tWHO IS HERE ?\n", DYELLOW);
	print_chan(srv, srv->clients[active]->chan, srv->clients[active]->csock);
	while (++i < srv->actual)
	{
		if (srv->clients[i]->chan != srv->clients[active]->chan)
			continue ;
		send_client(sock, "\t", NULL);
		send_client(sock, srv->clients[i]->nickname, NULL);
		send_client(sock, "\n", NULL);
	}
	send_client(sock, "\n", NULL);
}

void		me_cmd(t_server *srv, int active, char *message)
{
	int		sock;

	sock = srv->clients[active]->csock;
	if (!*message)
		return ;
	if (srv->clients[active]->chan == NO_CHAN)
	{
		send_client(sock, "You're not into a chan\n", DRED);
		send_client(sock, "Please join a chan to speak\n", DRED);
		send_client(sock, "Type /help to see a list of the chans\n", DRED);
		return ;
	}
	send_client(sock, srv->clients[active]->nickname, "\033[1;37m");
	send_client(sock, message, "\033[1;37m");
	send_client(sock, "\n", NULL);
	send_all_but_one(srv, active, srv->clients[active]->nickname, "\033[1;37m");
	send_all_but_one(srv, active, message, "\033[1;37m");
	send_all_but_one(srv, active, "\n", NULL);
	printf("[%s]", srv->clients[active]->chan);
	printf("\033[1;37m%s%s\033[0m\n", srv->clients[active]->nickname, message);
}

void		msg_cmd(t_server *srv, int actual, char *cmd)
{
	char	*name;
	char	*msg;
	int		i;
	int		sock;

	i = -1;
	while (cmd[++i] && cmd[i] != ' ')
		;
	sock = srv->clients[actual]->csock;
	if (!cmd[i])
	{
		send_client(sock, "Usage : /msg <name> <message>\n", DRED);
		return ;
	}
	cmd[i] = 0;
	name = cmd;
	msg = &cmd[i + 1];
	i = -1;
	while (++i < srv->actual)
	{
		if (!ft_strcmp(srv->clients[i]->nickname, name) && i != actual)
		{
			sock = srv->clients[i]->csock;
			send_client(sock, srv->clients[actual]->nickname, DBLUE);
			send_client(sock, " says : ", DBLUE);
			send_client(sock, msg, NULL);
			send_client(sock, "\n", NULL);
			sock = srv->clients[actual]->csock;
			send_client(sock, srv->clients[actual]->nickname, DGREEN);
			send_client(sock, " says : ", DBLUE);
			send_client(sock, msg, NULL);
			send_client(sock, "\n", NULL);
			return ;
		}
	}
	send_client(sock, name, DRED);
	send_client(sock, " isn't connected...\n", DRED);
}

void		srv_cmds(t_server *srv, char *cmd, int i)
{
	if (!ft_strncmp(cmd, "/nickname ", 9))
		change_nickname(srv, &cmd[10], i);
	else if (!ft_strncmp(cmd, "/nick ", 5))
		change_nickname(srv, &cmd[6], i);
	else if (!ft_strncmp(cmd, "/n ", 2))
		change_nickname(srv, &cmd[3], i);
	else if (!ft_strncmp(cmd, "/help", 4))
		welcome(srv, i);
	else if (!ft_strncmp(cmd, "/join ", 5))
		join_chan(srv, i, &cmd[6]);
	else if (!ft_strncmp(cmd, "/leave", 5))
		leave_chan(srv, i, 1);
	else if (!ft_strncmp(cmd, "/who", 3))
		who_cmd(srv, i);
	else if (!ft_strncmp(cmd, "/me ", 3))
		me_cmd(srv, i, &cmd[3]);
	else if (!ft_strncmp(cmd, "/msg ", 4))
		msg_cmd(srv, i, &cmd[5]);
	else
		send_client(srv->clients[i]->csock, "Bad command.\n", DRED);
}

void			kill_client(t_server *srv, int i)
{
	t_clients	*src;
	t_clients	*dest;
	size_t		size;

	printf("%sClient %s disconnected.%s\n", DRED, srv->clients[i]->nickname, NONE);
	send_all_but_one(srv, i, "Client ", DRED);
	send_all_but_one(srv, i, srv->clients[i]->nickname, DRED);
	send_all_but_one(srv, i, " disconnected.\n", DRED);
	close(srv->clients[i]->csock);
	free(srv->clients[i]->nickname);
	if (i < srv->actual - 1)
	{
		dest = srv->clients[i];
		src = srv->clients[i + 1];
		size = (srv->actual - i - 1) * sizeof(t_clients );
		memmove(dest, src, size);
	}
	else
		free(srv->clients[i]);
	srv->actual--;
}

void			print_message(t_server *srv, int i, char **message)
{
	int			j;
	int			csock;

	csock = srv->clients[i]->csock;
	send_client(csock, "\033[1A\r\033[K", NULL);
	if (!**message)
	{
		free(*message);
		return ;
	}
	if ((*message)[0] == '/' && (*message)[1] != '/')
	{
		srv_cmds(srv, *message, i);
		free(*message);
		return ;
	}
	if (srv->clients[i]->chan == NO_CHAN)
	{
		send_client(csock, "You're not into a chan\n", DRED);
		send_client(csock, "Please join a chan to speak\n", DRED);
		send_client(csock, "Type /help to see a list of the chans\n", DRED);
		free(*message);
		return ;
	}
	printf("[%s]%s : %s\n", srv->clients[i]->chan, srv->clients[i]->nickname, *message);
	j = -1;
	while (++j < srv->actual)
	{
		if (srv->clients[j]->chan != srv->clients[i]->chan)
			continue ;
		if (j == i)
			send_client(srv->clients[j]->csock, srv->clients[i]->nickname, DGREEN);
		else
			send_client(srv->clients[j]->csock, srv->clients[i]->nickname, DYELLOW);
		send_client(srv->clients[j]->csock, " : ", NULL);
		send_client(srv->clients[j]->csock, *message, NULL);
		send_client(srv->clients[j]->csock, "\033[4h\n\033[4l", NULL);
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
			FD_SET(srv->clients[i]->csock, &rdfs);
		if (select(srv->max + 1, &rdfs, NULL, NULL, NULL) == -1)
			ft_exit("Select Error\n");
		if (FD_ISSET(STDIN_FILENO, &rdfs))
			break ;
		else if (FD_ISSET(srv->sock, &rdfs))
		{
			srv->clients[srv->actual] = new_client(srv);
			if (srv->clients[srv->actual]->csock > srv->max)
				srv->max = srv->clients[srv->actual]->csock;
			FD_SET(srv->clients[srv->actual]->csock, &rdfs);
			printf("%s", DYELLOW);
			printf("%s connected.", srv->clients[srv->actual]->nickname);
			printf("%s\n", NONE);
			welcome(srv, srv->actual);
			srv->actual++;
		}
		else
		{
			i = -1;
			while (++i < srv->actual)
			{
				if (FD_ISSET(srv->clients[i]->csock, &rdfs))
				{
					message = get_message(srv->clients[i]->csock);
					if (!message || !ft_strcmp(message, "/quit") || !ft_strcmp(message, "/exit"))
					{
						send_client(srv->clients[i]->csock, "\033[1A\r\033[K", NULL);
						kill_client(srv, i);
					}
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

	close(server->sock);
/*	close(server->clients->csock);
	free(server->clients->nickname);*/
	free(server->clients);
	free(server);
	return (0);
}
