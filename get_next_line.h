/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmonfre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 11:25:09 by frmonfre          #+#    #+#             */
/*   Updated: 2023/03/03 08:40:21 by frmonfre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_elem
{
	char			*cont;
	struct s_elem	*next;
	struct s_elem	*prev;
}	t_elem;

struct s_lst
{
	t_elem	*head;
	t_elem	*tail;
	int		chrs;
	int		nwln;
	int		buff;
};

typedef struct s_lst	*t_lst;

char	*get_next_line(int fd);

void	extract_line(t_lst stash, char **line);

void	clean_stash(t_lst stash);

void	add_stash(t_lst stash, char *buf);

int		read_stash(int fd, t_lst stash);

int		ft_strlen(char *s);

int		fnd_new_line(t_lst stash);

int		extract_and_free(t_lst stash, char **line);

t_elem	*mng_nwln(t_lst stash);

void	init_stash(t_lst *stash);
#endif
