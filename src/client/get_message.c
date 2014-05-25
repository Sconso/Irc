/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:25:56 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 22:34:12 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <ft_fc_str.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
char				*get_message(int const fd)
{
	char			*msg;
	char			*buf;
	int				ret;

	if (!(buf = ft_strnew(BUFF_SIZE + 1)))
		return (NULL);
	if (!(msg = ft_strdup("")))
		return (NULL);
	while (42)
	{
		ret = recv(fd, buf, BUFF_SIZE, 0);
		if (ret <= 0)
			return (NULL);
		buf[ret] = 0;
		msg = ft_strcleanjoin(msg, buf);
		if (ret < BUFF_SIZE - 1)
		{
			free(buf);
			return (msg);
		}
	}
}
