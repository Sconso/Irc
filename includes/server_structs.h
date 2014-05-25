/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_structs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:26:36 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 19:27:10 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_STRUCTS_H
# define SERVER_STRUCTS_H

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
	t_clients			**cl;
}						t_server;

#endif
