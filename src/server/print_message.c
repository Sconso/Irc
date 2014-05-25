/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:02:28 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 23:43:42 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <ft_fc_str.h>
#include <stdlib.h>
#include <stdio.h>

static char		check_message(t_server *srv, int i, char **message, int csock)
{
	char		check;

	check = 0;
	if (!**message)
		++check;
	else if (!ft_strcmp(*message, "/gp") && ++check)
	{
		send_client(csock, "[", DBLUE, 0);
		send_client(csock, srv->cl[i]->chan, DBLUE, 0);
		send_client(csock, "]", DBLUE, 0);
		send_client(csock, " - ", NULL, 0);
		send_client(csock, "[", DRED, 0);
		send_client(csock, srv->cl[i]->nickname, DRED, 0);
		send_client(csock, "]", DRED, 0);
		send_client(csock, " : ", NULL, 1);
	}
	else if ((*message)[0] == '/' && (*message)[1] != '/' && ++check)
		srv_cmds(srv, *message, i);
	else if (srv->cl[i]->chan == NO_CHAN && ++check)
	{
		send_client(csock, "You're not into a chan\n", DRED, 0);
		send_client(csock, "Please join a chan to speak\n", DRED, 0);
		send_client(csock, "Type /help to see a list of the chans", DRED, 1);
	}
	return (check);
}

void			print_message(t_server *srv, int i, char **message)
{
	int			csock;

	csock = srv->cl[i]->csock;
	send_client(csock, "\033[1A\r\033[K", NULL, 0);
	if (check_message(srv, i, message, csock))
	{
		free(*message);
		return ;
	}
	printf("[%s]%s : %s\n", srv->cl[i]->chan, srv->cl[i]->nickname, *message);
	send_client(csock, srv->cl[i]->nickname, DGREEN, 0);
	send_client(csock, " : ", NULL, 0);
	send_client(csock, *message, NULL, 1);
	send_all_but_one(srv, i, srv->cl[i]->nickname, DYELLOW);
	send_all_but_one(srv, i, " : ", NULL);
	send_all_but_one(srv, i, *message, NULL);
	send_all_but_one(srv, i, "\0", NULL);
	free(*message);
}
