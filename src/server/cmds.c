/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 18:29:34 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:32:47 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <ft_fc_str.h>
#include <stdio.h>

static void		send_msg(t_server *srv, int actual, char *msg, int i)
{
	int			sock;

	sock = srv->cl[i]->csock;
	send_client(sock, srv->cl[actual]->nickname, DBLUE, 0);
	send_client(sock, " says : ", DBLUE, 0);
	send_client(sock, msg, NULL, 1);
	sock = srv->cl[actual]->csock;
	send_client(sock, "\033[1A\r\033[K", NULL, 0);
	send_client(sock, srv->cl[actual]->nickname, DGREEN, 0);
	send_client(sock, " says : ", DBLUE, 0);
	send_client(sock, msg, NULL, 1);
}

static void		msg_cmd(t_server *srv, int actual, char *cmd)
{
	char		*msg;
	int			i;
	int			sock;

	i = -1;
	while (cmd[++i] && cmd[i] != ' ')
		;
	sock = srv->cl[actual]->csock;
	if (!cmd[i])
		send_client(sock, "Usage : /msg <name> <message>", DRED, 1);
	if (!cmd[i])
		return ;
	cmd[i] = 0;
	msg = &cmd[i + 1];
	i = -1;
	while (++i < srv->actual)
	{
		if (!ft_strcmp(srv->cl[i]->nickname, cmd) && i != actual)
		{
			send_msg(srv, actual, msg, i);
			return ;
		}
	}
	send_client(sock, cmd, DRED, 0);
	send_client(sock, " isn't connected...", DRED, 1);
}

static void		who_cmd(t_server *srv, int active)
{
	int			sock;
	int			i;

	sock = srv->cl[active]->csock;
	i = -1;
	send_client(sock, "\n\tWHO IS HERE ?\n", DYELLOW, 0);
	print_chan(srv, srv->cl[active]->chan, srv->cl[active]->csock);
	while (++i < srv->actual)
	{
		if (srv->cl[i]->chan != srv->cl[active]->chan)
			continue ;
		send_client(sock, "\t", NULL, 0);
		send_client(sock, srv->cl[i]->nickname, NULL, 0);
		send_client(sock, "\n", NULL, 0);
	}
	send_client(sock, "\b\0", NULL, 1);
}

static void		me_cmd(t_server *srv, int active, char *message)
{
	int			sock;

	sock = srv->cl[active]->csock;
	if (!*message)
		return ;
	if (srv->cl[active]->chan == NO_CHAN)
	{
		send_client(sock, "You're not into a chan\n", DRED, 0);
		send_client(sock, "Please join a chan to speak\n", DRED, 0);
		send_client(sock, "Type /help to see a list of the chans", DRED, 1);
		return ;
	}
	send_client(sock, srv->cl[active]->nickname, "\033[1;37m", 0);
	send_client(sock, message, "\033[1;37m", 1);
	send_all_but_one(srv, active, srv->cl[active]->nickname, "\033[1;37m");
	send_all_but_one(srv, active, message, "\033[1;37m");
	send_all_but_one(srv, active, "\0", NULL);
	printf("[%s]", srv->cl[active]->chan);
	printf("\033[1;37m%s%s\033[0m\n", srv->cl[active]->nickname, message);
}

void			srv_cmds(t_server *srv, char *cmd, int i)
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
		send_client(srv->cl[i]->csock, "Bad command.", DRED, 1);
}
