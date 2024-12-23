/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:07:14 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/17 20:38:04 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_pipex(char **argv, int *fd, int *file_in, int *file_out)
{
	*file_in = open(argv[1], O_RDONLY);
	if (*file_in < 0)
	{
		perror("Error opening file1");
		return (1);
	}
	*file_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*file_out < 0)
	{
		perror("Error opening file2");
		close(*file_in);
		return (2);
	}
	if (pipe(fd) == -1)
	{
		perror("Pipe failed");
		close(*file_in);
		close(*file_out);
		return (3);
	}
	return (0);
}

int	ft_check_fork(int pid, int *fd, int *file_in, int *file_out)
{
	if (pid < 0)
	{
		perror("fork error");
		ft_close_all(fd, file_in, file_out);
		return (1);
	}
	return (0);
}
