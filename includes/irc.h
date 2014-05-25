/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 18:17:37 by sconso            #+#    #+#             */
/*   Updated: 2014/05/24 22:59:37 by Myrkskog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_H
# define IRC_H

# include <arpa/inet.h>

typedef int						t_socket;
typedef struct sockaddr_in		t_sockaddr_in;
typedef struct sockaddr			t_sockaddr;
typedef struct in_addr			t_in_addr;

/*
** ft_errors.c
*/
void	ft_assert(int test, char *str);
void	ft_exit(char *str);
void	send_client(int sock, char *str, char *color);

#endif
