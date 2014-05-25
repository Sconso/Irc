/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 18:19:08 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:23:25 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>

static void		print_cmds(int cl)
{
	send_client(cl, "COMMANDS :\n", DRED, 0);
	send_client(cl, "\t/n, /nick, /nickname <name> : ", DGREEN, 0);
	send_client(cl, "Select a new nickname.\n", NULL, 0);
	send_client(cl, "\t/join <chan> : ", DGREEN, 0);
	send_client(cl, "Join a chan.\n", NULL, 0);
	send_client(cl, "\t/leave : ", DGREEN, 0);
	send_client(cl, "Leave the current chan.\n", NULL, 0);
	send_client(cl, "\t/msg <who> <msg> : ", DGREEN, 0);
	send_client(cl, "Send a private message.\n", NULL, 0);
	send_client(cl, "\t/who : ", DGREEN, 0);
	send_client(cl, "Show who is on this chan.\n", NULL, 0);
	send_client(cl, "\t/me <message> : ", DGREEN, 0);
	send_client(cl, "Send a roleplay formated message\n", NULL, 0);
	send_client(cl, "\t/exit, /quit : ", DGREEN, 0);
	send_client(cl, "Leave the client.\n", NULL, 0);
	send_client(cl, "\t/help : ", DGREEN, 0);
	send_client(cl, "Print this message.\n", NULL, 0);
}

void			print_chan(t_server *srv, char *chan, int cl)
{
	int			nb;
	int			i;
	char		*nb_char;

	if (chan == 0)
		return ;
	nb = 0;
	i = -1;
	while (++i < srv->actual)
		nb += (srv->cl[i]->chan == chan ? 1 : 0);
	nb_char = ft_itoa(nb);
	send_client(cl, "\t", DGREEN, 0);
	send_client(cl, chan, DGREEN, 0);
	send_client(cl, " : ", NULL, 0);
	send_client(cl, nb_char, NULL, 0);
	send_client(cl, " Connected now.\n", NULL, 0);
	free(nb_char);
}

void			welcome(t_server *srv, int active)
{
	int			cl;

	cl = srv->cl[active]->csock;
	send_client(cl, "\tWelcome to SC_IRC ", DGREEN, 0);
	send_client(cl, srv->cl[active]->nickname, DGREEN, 0);
	send_client(cl, "\n\nBe polite, respectfull and don't panic.\n\n", NULL, 0);
	print_cmds(cl);
	send_client(cl, "\nActive Chans :\n", DRED, 0);
	print_chan(srv, MUSIC, cl);
	print_chan(srv, CODING, cl);
	print_chan(srv, GAMES, cl);
	send_client(cl, "\nAnd now... Have fun !", NULL, 1);
}
