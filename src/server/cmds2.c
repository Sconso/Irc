/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 18:42:02 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:26:16 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <ft_fc_str.h>
#include <stdio.h>
#include <stdlib.h>

void		change_nickname(t_server *srv, char *nickname, int i)
{
	int		size;
	int		sock;

	size = ft_strlen(nickname);
	sock = srv->cl[i]->csock;
	if (size <= 0 || size > 9)
	{
		send_client(sock,
				"Nickname must be between 1 and 9 char long.", DRED, 1);
		return ;
	}
	printf("%s%s %s%s ", DYELLOW, "Client", DRED, srv->cl[i]->nickname);
	printf("%s%s %s%s", DYELLOW, "changed nickname to", DRED, nickname);
	printf("%s.\n", NONE);
	send_all_but_one(srv, i, "Client ", DYELLOW);
	send_all_but_one(srv, i, srv->cl[i]->nickname, DRED);
	send_all_but_one(srv, i, " changed nickname to ", DYELLOW);
	send_all_but_one(srv, i, nickname, DRED);
	send_all_but_one(srv, i, ".", DYELLOW);
	send_all_but_one(srv, i, "\0", NULL);
	send_client(sock, "Successfully changed Nickname.", DYELLOW, 1);
	free(srv->cl[i]->nickname);
	srv->cl[i]->nickname = ft_strdup(nickname);
}

void		leave_chan(t_server *srv, int active, char warn)
{
	int		sock;

	sock = srv->cl[active]->csock;
	if (srv->cl[active]->chan == NO_CHAN)
	{
		if (warn)
			send_client(sock, "You aren't on a chan.", DRED, 1);
		return ;
	}
	send_client(sock, "You left ", DYELLOW, 0);
	send_client(sock, srv->cl[active]->chan, DYELLOW, 1);
	send_all_but_one(srv, active, srv->cl[active]->nickname, DRED);
	send_all_but_one(srv, active, " left the chan", DRED);
	send_all_but_one(srv, active, "\0", DRED);
	printf("%s%s left ", DYELLOW, srv->cl[active]->nickname);
	printf("%s%s\n", srv->cl[active]->chan, NONE);
	srv->cl[active]->chan = NO_CHAN;
}

void		join_chan(t_server *srv, int cl, char *chan)
{
	int		sock;

	leave_chan(srv, cl, 0);
	sock = srv->cl[cl]->csock;
	if (!ft_strcmp(chan, MUSIC))
		srv->cl[cl]->chan = MUSIC;
	else if (!ft_strcmp(chan, CODING))
		srv->cl[cl]->chan = CODING;
	else if (!ft_strcmp(chan, GAMES))
		srv->cl[cl]->chan = GAMES;
	else
	{
		send_client(sock, "Not a valid chan.\n", DRED, 0);
		send_client(sock, "Type /help to see the chans.", DRED, 1);
		return ;
	}
	send_client(sock, "\n\tWelcome to ", DYELLOW, 0);
	send_client(sock, chan, DYELLOW, 0);
	send_client(sock, ", have fun !", DYELLOW, 1);
	send_all_but_one(srv, cl, srv->cl[cl]->nickname, DYELLOW);
	send_all_but_one(srv, cl, " connected.", DYELLOW);
	send_all_but_one(srv, cl, "\0", DYELLOW);
	printf("%s%s joined ", DYELLOW, srv->cl[cl]->nickname);
	printf("%s%s\n", chan, NONE);
}
