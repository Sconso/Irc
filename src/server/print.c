/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 18:19:08 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 18:28:58 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <stdlib.h>
#include <ft_fc_conversion.h>

static void		print_cmds(int cl)
{
	send_client(cl, "COMMANDS :\n", DRED);
	send_client(cl, "\t/n, /nick, /nickname <name> : ", DGREEN);
	send_client(cl, "Select a new nickname.\n", NULL);
	send_client(cl, "\t/join <chan> : ", DGREEN);
	send_client(cl, "Join a chan.\n", NULL);
	send_client(cl, "\t/leave : ", DGREEN);
	send_client(cl, "Leave the current chan.\n", NULL);
	send_client(cl, "\t/msg <who> <msg> : ", DGREEN);
	send_client(cl, "Send a private message.\n", NULL);
	send_client(cl, "\t/who : ", DGREEN);
	send_client(cl, "Show who is on this chan.\n", NULL);
	send_client(cl, "\t/me <message> : ", DGREEN);
	send_client(cl, "Send a roleplay formated message\n", NULL);
	send_client(cl, "\t/exit, /quit : ", DGREEN);
	send_client(cl, "Leave the client.\n", NULL);
	send_client(cl, "\t/help : ", DGREEN);
	send_client(cl, "Print this message.\n", NULL);
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
	send_client(cl, "\t", DGREEN);
	send_client(cl, chan, DGREEN);
	send_client(cl, " : ", NULL);
	send_client(cl, nb_char, NULL);
	send_client(cl, " Connected now.\n", NULL);
	free(nb_char);
}

void			welcome(t_server *srv, int active)
{
	int			cl;

	cl = srv->cl[active]->csock;
	send_client(cl, "\tWelcome to SC_IRC ", DGREEN);
	send_client(cl, srv->cl[active]->nickname, DGREEN);
	send_client(cl, "\n\nBe polite, respectfull and don't panic.\n\n", NULL);
	print_cmds(cl);
	send_client(cl, "\nActive Chans :\n", DRED);
	print_chan(srv, MUSIC, cl);
	print_chan(srv, CODING, cl);
	print_chan(srv, GAMES, cl);
	send_client(cl, "\nAnd now... Have fun !\n", "\0");
}
