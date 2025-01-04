/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:28:51 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/04 20:55:10 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_open_fd(t_fd_pipe *fd_pipe, int argc, char **argv)
{
	fd_pipe->in_fd = open(argv[1], O_RDONLY);
	if (fd_pipe->in_fd < 0)
	{
		write(2, "Error opening file1\n", 20);
		//return (FILE_ERROR);
	}
	fd_pipe->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_pipe->out_fd < 0)
	{
		perror("Error opening file2\n");
		close(fd_pipe->in_fd);
		return (FILE_ERROR);
	}
	if (pipe(fd_pipe->fd) == -1)
	{
		perror("Pipe failed\n");
		close(fd_pipe->in_fd);
		close(fd_pipe->out_fd);
		return (PIPE_ERROR);
	}
	return (SUCCESS);
}
