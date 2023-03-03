/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmonfre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 11:24:46 by frmonfre          #+#    #+#             */
/*   Updated: 2023/03/02 12:18:17 by frmonfre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_lst	nw_t_lst(int fd, t_lst *files)
{
	t_lst	stash;

	stash = (t_lst) malloc(sizeof(struct s_lst));
	if (!stash)
		return (NULL);
	stash->tail = NULL;
	stash->head = NULL;
	stash->nwln = 0;
	stash->chrs = 0;
	stash->buff = 0;
	files[fd] = stash;
	return (stash);
}

int	ft_strlen(char *s)
{
	int		i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	fnd_new_line(t_lst stash)
{
	int		i;

	if (stash == NULL)
		return (0);
	if (stash->head == NULL)
		return (0);
	i = 0;
	while (stash->tail->cont[i] && stash->tail->cont[i] != '\n')
		i++;
	if (!stash->tail->cont[i])
		return (0);
	return (1);
}

int	extract_and_free(t_lst stash, char **line)
{
	int		i;
	int		ln;
	t_elem	*el;

	el = stash->head;
	ln = 0;
	while (el)
	{
		i = 0;
		while (el->cont[i] && el->cont[i] != '\n')
			line[0][ln++] = el->cont[i++];
		if (el->cont[i] == '\n')
		{
			line[0][ln] = '\n';
			ln++;
			stash->nwln = i + 1;
		}
		el = el->next;
		if (el != NULL)
		{
			free(el->prev->cont);
			free(el->prev);
		}
	}
	return (ln);
}

t_elem	*mng_nwln(t_lst stash)
{
	int		i;
	t_elem	*nw;

	i = 0;
	nw = (t_elem *) malloc(sizeof(t_elem));
	if (!nw)
		return (NULL);
	nw->cont = (char *) malloc(sizeof(char) * (stash->buff + 1));
	if (!nw->cont)
		return (NULL);
	nw->next = NULL;
	nw->prev = NULL;
	stash->chrs = stash->buff;
	while (stash->tail->cont[stash->nwln + i])
	{
		nw->cont[i] = stash->tail->cont[stash->nwln + i];
		i++;
	}
	nw->cont[i] = '\0';
	return (nw);
}
