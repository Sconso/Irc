/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Myrkskog <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/24 20:57:09 by Myrkskog          #+#    #+#             */
/*   Updated: 2014/05/24 21:01:10 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>
#include <ft_fc_str.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

static char			*ft_exit_gm(char **str)
{
	free(*str);
	*str = 0;
	return (0);
}

char				*get_message(int const fd)
{
	static char		*tmp = 0;
	char			*buf;
	char			*size;
	int				ret;

	if (fd < 0 || BUFF_SIZE < 1 || !(buf = ft_strnew(BUFF_SIZE + 1)))
		return (ft_exit_gm(&tmp));
	tmp = (tmp ? tmp : ft_strdup(""));
	while (!(size = ft_strchr(tmp, '\n'))
			&& (ret = recv(fd, buf, BUFF_SIZE, 0)))
	{
		buf[ret] = 0;
		tmp = ft_strcleanjoin(tmp, buf);
	}
	free(buf);
	if (tmp[0] && (size || (size = ft_strchr(tmp, 0))))
	{
		buf = ft_strsub(tmp, 0, size - tmp);
		size = (size[0] ? ft_strdup(size + 1) : ft_strdup(""));
		free(tmp);
		tmp = size;
		return (buf);
	}
	return (ft_exit_gm(&tmp));
}
