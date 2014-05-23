/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_dlist_structs.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/18 23:17:00 by sconso            #+#    #+#             */
/*   Updated: 2014/05/18 23:17:56 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FC_DLIST_STRUCTS_H
# define FT_FC_DLIST_STRUCTS_H

typedef struct			s_dlist
{
	char				*name;
	struct s_dlist		*next;
	struct s_dlist		*prev;
}						t_dlist;

typedef struct			s_index
{
	unsigned int		size;
	t_dlist				*first;
	t_dlist				*last;
}						t_index;

enum					e_direction
{
	FORWARD, BACKWARD
};

#endif
