/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 18:17:37 by sconso            #+#    #+#             */
/*   Updated: 2014/05/23 18:29:23 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_H
# define IRC_H

typedef int						t_socket;
typedef struct sockaddr_in		t_sockadd_in;
typedef struct sockaddr			t_sockaddr;
typedef struct in_addr			t_in_addr;

typedef struct					s_clients
{
	char						*nickname;
}								t_clients;

#endif
