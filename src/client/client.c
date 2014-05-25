/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:27:52 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 23:44:08 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>
#include <get_next_line.h>
#include <ft_fc_conversion.h>
#include <ft_fc_str.h>
#include <ft_fc_print.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void			init_socks(t_socket sock, fd_set *rdfs)
{
	FD_ZERO(rdfs);
	FD_SET(STDIN_FILENO, rdfs);
	FD_SET(sock, rdfs);
	if (select(sock + 1, rdfs, NULL, NULL, NULL) == -1)
		ft_exit("Select error\n");
}

static void			client_in(t_socket sock)
{
	char			*message;

	if ((message = get_next_line(STDIN_FILENO)) != NULL)
	{
		if (!strcmp(message, "/exit") || !strcmp(message, "/quit"))
		{
			free(message);
			ft_exit("Client exited\n");
		}
		message = ft_strcleanjoin(message, "\n");
		ft_send(sock, message);
		free(message);
	}
}

static void			client_out(t_socket sock)
{
	char			*message;

	if ((message = get_message(sock)) != NULL)
	{
		printf("\r\033[K%s\n", message);
		free(message);
	}
	else
		ft_exit("Server disconnected\n");
}

void				be_client(char *ip, int port)
{
	t_socket		sock;
	fd_set			rdfs;
	char			*ask;
	char			*prompt;

	sock = init_connection(ip, port);
	while (1)
	{
		init_socks(sock, &rdfs);
		if (FD_ISSET(STDIN_FILENO, &rdfs))
			client_in(sock);
		else if (FD_ISSET(sock, &rdfs))
		{
			client_out(sock);
			ask = ft_strdup("/gp\n");
			ft_send(sock, ask);
			free(ask);
			prompt = get_message(sock);
			ft_putstr("\n");
			ft_putstr(prompt);
			free(prompt);
		}
	}
}

int					main(int ac, char **av)
{
	if (ac < 3)
		ft_exit("\033[1;33mUsage : ./client <ip> <port> [nickname]\033[0m\n");
	init_client(ac, av);
	return (0);
}
