/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:01:46 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 17:15:12 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>
#include <get_next_line.h>
#include <ft_fc_conversion.h>
#include <ft_fc_str.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void				ft_send(int sock, char *message)
{
	if (send(sock, message, ft_strlen(message), 0) < 0)
		ft_exit("Send error\n");
}

int					init_connection(char *ip, int port)
{
	t_socket		sock;
	t_sockaddr_in	sin;
	struct hostent	*hostinfo;

	printf("%s - %d\n", ip, port);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	ft_assert(sock != -1, "Socket Error\n");
	hostinfo = gethostbyname(ip);
	ft_assert(hostinfo != NULL, "Unknown Host.\n");
	sin.sin_addr = *(t_in_addr *)hostinfo->h_addr;
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;
	if (connect(sock, (t_sockaddr *)&sin, sizeof(t_sockaddr)) == -1)
		ft_exit("Connect Error\n");
	return (sock);
}

void			be_client(char *ip, int port)
{
	t_socket	sock;
	fd_set		rdfs;
	char		*message;

	sock = init_connection(ip, port);
	while (1)
	{
		FD_ZERO(&rdfs);
		FD_SET(STDIN_FILENO, &rdfs);
		FD_SET(sock, &rdfs);
		if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
			ft_exit("Select error\n");
		if (FD_ISSET(STDIN_FILENO, &rdfs))
		{
			if ((message = get_next_line(STDIN_FILENO)) != NULL)
			{
				printf("%s\n", message);
				ft_send(sock, message);
				free(message);
			}
		}
		else if (FD_ISSET(sock, &rdfs))
		{
			while ((message = get_message(sock)) != NULL)
			{
				printf("%s\n", message);
				free(message);
			}
		}
	}
}

void		init_client(int ac, char **av)
{
	int		port;
	char	*ip;
	char	*name;

	port = ft_atoi(av[2]);
	if (!ft_strcmp(av[1], "localhost"))
		ip = ft_strdup("127.0.0.1");
	else
		ip = ft_strdup(av[1]);
	name = (ac >= 4 ? av[3] : NULL);
	be_client(ip, port);
}

int			main(int ac, char **av)
{
	if (ac < 3)
		ft_exit("\033[1;33mUsage : ./client <ip> <port> [nickname]\033[0m\n");
	init_client(ac, av);
	return (0);
}
