/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Myrkskog <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/24 20:39:22 by Myrkskog          #+#    #+#             */
/*   Updated: 2014/05/25 10:30:08 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <irc.h>
# define BUFF_SIZE (200)
# define MAX_CLIENTS (42)

# define NO_CHAN (0)
# define MUSIC ("Music")
# define CODING ("Coding")
# define GAMES ("Games")

typedef struct			s_clients
{
	t_socket			csock;
	t_sockaddr_in		csin;
	char				*nickname;
	char				*chan;
}						t_clients;

typedef struct			s_server
{
	t_socket			sock;
	t_sockaddr_in		sin;
	int					max_clients;
	int					max;
	int					actual;
	t_clients			**clients;
}						t_server;

/*
** get_message.c
*/
char	*get_message(int const fd);
#endif
