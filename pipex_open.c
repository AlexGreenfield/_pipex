/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:28:51 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/13 18:17:22 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_open_fd(t_fd_pipe *fd_pipe, int argc, char **argv)
{
	int	pipe_result;

	fd_pipe->in_fd = open(argv[1], O_RDONLY);
	fd_pipe->out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pipe_result = pipe(fd_pipe->fd);
	if (fd_pipe->in_fd < 0)
		perror("Error opening infile fd");
	if (fd_pipe->out_fd < 0)
	{
		perror(argv[5]);
		return (FILE_ERROR);
	}
	if (pipe_result == -1)
	{
		perror("Pipe failed\n");
		return (PIPE_ERROR);
	}
	return (SUCCESS);
}
