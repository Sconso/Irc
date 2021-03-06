/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/25 19:25:12 by sconso            #+#    #+#             */
/*   Updated: 2014/05/25 19:38:11 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <irc.h>

/*
** client.h
*/
void	be_client(char *ip, int port);

/*
** ft_send.h
*/
void	ft_send(int sock, char *message);

/*
** init.c
*/
int		init_connection(char *ip, int port);
void	init_client(int ac, char **av);

#endif
