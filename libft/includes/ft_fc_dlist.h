/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_dlist.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sconso <sconso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/12 18:17:53 by sconso            #+#    #+#             */
/*   Updated: 2014/05/18 23:17:24 by sconso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FC_DLIST_H
# define FT_FC_DLIST_H

# include <ft_fc_dlist_structs.h>

t_index			*ft_createdlist(void);
t_dlist			*ft_addfirstdlist(t_index *list, char *data);
t_dlist			*ft_addlastdlist(t_index *list, char *data);
t_dlist			*ft_addindlist(t_index *list, char *data, unsigned int pos);
t_dlist			*ft_addalphaindlist(t_index *list, char *data);
t_dlist			*ft_addralphaindlist(t_index *list, char *data);
t_index			*ft_deletefirstdlist(t_index *list);
t_index			*ft_deletelastdlist(t_index *list);
t_index			*ft_deleteindlist(t_index *list, unsigned int position);
t_index			*ft_flushdlist(t_index *list);
void			ft_readdlist(t_index *list, char direction);

#endif
