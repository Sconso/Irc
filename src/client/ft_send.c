/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_send.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:37:13 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 20:46:53 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>
#include <ft_fc_str.h>

void	ft_send(t_socket sock, char *message)
{
	if (send(sock, message, ft_strlen(message), 0) < 0)
		ft_exit("Send error\n");
}
