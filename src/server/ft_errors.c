/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:20:51 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:34:29 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_fc_print.h>
#include <ft_fc_str.h>
#include <colors.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void			ft_exit(char *str)
{
	ft_putstr_fd(str, 2);
	exit(0);
}

void			ft_assert(int test, char *str)
{
	if (!test)
		ft_exit(str);
}

void			send_client(int sock, char *str, char *color, char ok)
{
	static char		*msg = NULL;

	msg = (msg ? msg : ft_strdup(""));
	if (color)
		msg = ft_strcleanjoin(msg, color);
	msg = ft_strcleanjoin(msg, str);
	if (color)
		msg = ft_strcleanjoin(msg, NONE);
	if (ok)
	{
		send(sock, msg, ft_strlen(msg), 0);
		free(msg);
		msg = NULL;
	}
}
