/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmonfre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 11:28:02 by frmonfre          #+#    #+#             */
/*   Updated: 2023/03/02 12:06:33 by frmonfre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	extract_line(t_lst stash, char **line)
{
	int		ln;

	if (stash == NULL)
		return ;
	*line = (char *) malloc(sizeof(char) * (stash->chrs + 1));
	if (!*line)
		return ;
	ln = extract_and_free(stash, line);
	stash->head = stash->tail;
	stash->buff = stash->chrs - ln;
	line[0][ln] = '\0';
}

void	clean_stash(t_lst stash)
{
	int		i;
	t_elem	*nw;

	if (stash == NULL)
		return ;
	i = 0;
	if (stash->nwln)
		nw = mng_nwln(stash);
	if (stash->tail != NULL)
	{
		free(stash->tail->cont);
		free(stash->tail);
	}
	if (stash->nwln)
	{
		stash->head = nw;
		stash->tail = nw;
	}
	else
	{
		stash->head = NULL;
		stash->tail = NULL;
		stash->chrs = 0;
	}
	stash->nwln = 0;
}

void	add_stash(t_lst stash, char *buf)
{
	int		i;
	t_elem	*nw;

	nw = (t_elem *) malloc(sizeof(t_elem));
	if (!nw)
		return ;
	nw->cont = (char *) malloc(sizeof(char) * (ft_strlen(buf) + 1));
	if (!nw->cont)
		return ;
	i = 0;
	while (buf[i])
	{
		nw->cont[i] = buf[i];
		i++;
		stash->chrs++;
	}
	nw->cont[i] = '\0';
	nw->next = NULL;
	nw->prev = stash->tail;
	if (stash->tail != NULL)
		stash->tail->next = nw;
	stash->tail = nw;
	if (stash->head == NULL)
		stash->head = nw;
}

int	read_stash(int fd, t_lst stash)
{
	char	*buf;
	int		byte;

	byte = 1;
	buf = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
	while (!fnd_new_line(stash) && byte > 0)
	{
		if (!buf)
			return (1);
		byte = (int) read(fd, buf, BUFFER_SIZE);
		if (byte <= 0)
		{
			free(buf);
			return (1);
		}
		buf[byte] = '\0';
		add_stash(stash, buf);
	}
	free(buf);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_lst	files[1024];
	t_lst			stash;
	char			*line;
	int				end;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, &line, 0) < 0)
		return (NULL);
	if (files[fd] != NULL)
		stash = files[fd];
	else
		stash = nw_t_lst(fd, files);
	end = read_stash(fd, stash);
	extract_line(stash, &line);
	clean_stash(stash);
	if (end)
	{
		free(files[fd]);
		files[fd] = NULL;
	}
	if (!line[0])
	{
		free(line);
		return (NULL);
	}
	return (line);
}
