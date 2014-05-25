/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:08:31 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 19:20:07 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <colors.h>
#include <ft_fc_conversion.h>
#include <ft_fc_str.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void				kill_all_clients(t_server *srv)
{
	int				i;

	i = -1;
	while (++i < srv->actual)
	{
		close(srv->cl[i]->csock);
		free(srv->cl[i]->nickname);
		free(srv->cl[i]);
	}
}

void				kill_client(t_server *srv, int i)
{
	t_clients		*src;
	t_clients		*dest;
	size_t			size;

	printf("%sClient %s disconnected.%s\n", DRED, srv->cl[i]->nickname, NONE);
	send_all_but_one(srv, i, "Client ", DRED);
	send_all_but_one(srv, i, srv->cl[i]->nickname, DRED);
	send_all_but_one(srv, i, " disconnected.\n", DRED);
	close(srv->cl[i]->csock);
	free(srv->cl[i]->nickname);
	if (i < srv->actual - 1)
	{
		dest = srv->cl[i];
		src = srv->cl[i + 1];
		size = (srv->actual - i - 1) * sizeof(t_clients);
		memmove(dest, src, size);
	}
	else
		free(srv->cl[i]);
	srv->actual--;
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

void				send_all_but_one(t_server *srv, int one, char *msg,
									char *color)
{
	int		i;
	int		sock;

	i = -1;
	while (++i < srv->actual)
	{
		sock = srv->cl[i]->csock;
		if (srv->cl[i]->chan != srv->cl[one]->chan)
			continue ;
		if (i != one)
			send_client(sock, msg, color);
	}
}
