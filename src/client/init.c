/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:28:45 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 20:58:33 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>
#include <ft_fc_conversion.h>
#include <ft_fc_str.h>
#include <netdb.h>
#include <stdio.h>

int						init_connection(char *ip, int port)
{
	t_socket			sock;
	t_sockaddr_in		sin;
	struct hostent		*hostinfo;

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

void					init_client(int ac, char **av)
{
	int					port;
	char				*ip;
	char				*name;

	(void)name;
	port = ft_atoi(av[2]);
	if (!ft_strcmp(av[1], "localhost"))
		ip = ft_strdup("127.0.0.1");
	else
		ip = ft_strdup(av[1]);
	name = (ac >= 4 ? av[3] : NULL);
	be_client(ip, port);
}
