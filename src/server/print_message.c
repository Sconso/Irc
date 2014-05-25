/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:02:28 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 19:08:07 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <stdlib.h>
#include <stdio.h>

static char		check_message(t_server *srv, int i, char **message, int csock)
{
	char		check;

	check = 0;
	if (!**message)
		++check;
	if ((*message)[0] == '/' && (*message)[1] != '/' && ++check)
		srv_cmds(srv, *message, i);
	if (srv->cl[i]->chan == NO_CHAN && ++check)
	{
		send_client(csock, "You're not into a chan\n", DRED);
		send_client(csock, "Please join a chan to speak\n", DRED);
		send_client(csock, "Type /help to see a list of the chans\n", DRED);
	}
	return (check);
}

void			print_message(t_server *srv, int i, char **message)
{
	int			j;
	int			csock;

	csock = srv->cl[i]->csock;
	send_client(csock, "\033[1A\r\033[K", NULL);
	if (check_message(srv, i, message, csock))
	{
		free(*message);
		return ;
	}
	printf("[%s]%s : %s\n", srv->cl[i]->chan, srv->cl[i]->nickname, *message);
	j = -1;
	while (++j < srv->actual)
	{
		if (srv->cl[j]->chan != srv->cl[i]->chan)
			continue ;
		if (j == i)
			send_client(srv->cl[j]->csock, srv->cl[i]->nickname, DGREEN);
		else
			send_client(srv->cl[j]->csock, srv->cl[i]->nickname, DYELLOW);
		send_client(srv->cl[j]->csock, " : ", NULL);
		send_client(srv->cl[j]->csock, *message, NULL);
		send_client(srv->cl[j]->csock, "\033[4h\n\033[4l", NULL);
	}
	free(*message);
}
