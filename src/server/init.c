/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 18:24:34 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 18:28:06 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <ft_fc_conversion.h>
#include <stdlib.h>

int				check_validity(int ac, char **av)
{
	int			port;
	int			max_clients;

	if (ac < 2)
		ft_exit("\033[1;33mUsage : ./server <port> [max_clients]\033[0m\n");
	port = ft_atoi(av[1]);
	max_clients = (ac >= 3 ? ft_atoi(av[2]) : MAX_CLIENTS);
	max_clients = (max_clients <= 0 ? MAX_CLIENTS : max_clients);
	if (port <= 0)
		ft_exit("\033[1;33mInvalid Port\033[0m\n");
	return (max_clients);
}

t_server		*init_server(char *port)
{
	t_server	*serv;

	serv = (t_server *)malloc(sizeof(t_server));
	ft_assert(serv != NULL, "Can't create server\n");
	serv->sock = socket(AF_INET, SOCK_STREAM, 0);
	ft_assert(serv->sock != -1, "Socket Error\n");
	serv->sin.sin_family = AF_INET;
	serv->sin.sin_addr.s_addr = INADDR_ANY;
	serv->sin.sin_port = htons(ft_atoi(port));
	if (bind(serv->sock, (t_sockaddr *)&serv->sin, sizeof(serv->sin)) == -1)
		ft_exit("Bind Error\n");
	if (listen(serv->sock, serv->max_clients) == -1)
		ft_exit("Listen Problem\n");
	serv->cl = (t_clients **)malloc(serv->max_clients * sizeof(t_clients *));
	serv->max = serv->sock;
	serv->actual = 0;
	return (serv);
}
