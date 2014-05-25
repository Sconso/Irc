/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:25:40 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:51:52 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <irc.h>
# include <server_structs.h>

# define BUFF_SIZE (2000)
# define MAX_CLIENTS (42)

# define NO_CHAN ("Lobby")
# define MUSIC ("Music")
# define CODING ("Coding")
# define GAMES ("Games")

/*
** client.c
*/
void		kill_all_clients(t_server *srv);
void		kill_client(t_server *srv, int i);
t_clients	*new_client(t_server *srv);
void		send_all_but_one(t_server *srv, int one, char *msg, char *color);

/*
** cmds.c
*/
void		srv_cmds(t_server *srv, char *cmd, int i);

/*
** cmds2.c
*/
void		change_nickname(t_server *srv, char *nickname, int i);
void		leave_chan(t_server *srv, int active, char warn);
void		join_chan(t_server *srv, int cl, char *chan);

/*
** init.c
*/
int			check_validity(int ac, char **av);
t_server	*init_server(char *port);

/*
** print.c
*/
void		print_chan(t_server *srv, char *chan, int cl);
void		welcome(t_server *srv, int active);

/*
** print_message.c
*/
void		print_message(t_server *srv, int i, char **message);

#endif
