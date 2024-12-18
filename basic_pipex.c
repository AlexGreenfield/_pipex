/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:59:58 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/18 19:26:24 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int			ft_pipex(char **argv);
static int	child_1(char **argv, int *fd, int *file_in, int *file_out);
static int	child_2(char **argv, int *fd, int *file_in, int *file_out);
int			ft_check_pipex(char **argv, int *fd, int *file_in, int *file_out);
void		ft_close_all(int *fd, int *file_in, int *file_out);

int	ft_pipex(char **argv)
{
	int	fd[2];
	int	file_in;
	int	file_out;

	if (ft_check_pipex(argv, fd, &file_in, &file_out) != 0)
		return (1);
	if (child_1(argv, fd, &file_in, &file_out) != 0)
	{
		ft_close_all(fd, &file_in, &file_out);
		return (1);
	}
	if (child_2(argv, fd, &file_in, &file_out) != 0)
	{
		ft_close_all(fd, &file_in, &file_out);
		return (1);
	}
	return (0);
}

static int	child_1(char **argv, int *fd, int *file_in, int *file_out)
{
	int	pid1;

	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
	{
		if (dup2(*file_in, STDIN_FILENO) < 0)
		{
			perror("Error duplicating file_in");
			exit(1);
		}
		if (dup2(fd[1], STDOUT_FILENO) < 0)
		{
			perror("Error duplicating pipe write end");
			exit(1);
		}
		ft_close_all(fd, file_in, file_out);
		execlp("/bin/sh", "sh", "-c", argv[2], NULL);
		perror("Error executing cmd1");
		exit(1);
	}
	waitpid(pid1, NULL, 0);
	return (0);
}

static int	child_2(char **argv, int *fd, int *file_in, int *file_out)
{
	int	pid2;

	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) < 0)
		{
			perror("Error duplicating pipe write beggining");
			exit(1);
		}
		if (dup2(*file_out, STDOUT_FILENO) < 0)
		{
			perror("Error duplicating file_out");
			exit(1);
		}
		ft_close_all(fd, file_in, file_out);
		execlp("/bin/sh", "sh", "-c", argv[3], NULL);
		perror("Error executing cmd2");
		exit(1);
	}
	ft_close_all(fd, file_in, file_out);
	waitpid(pid2, NULL, 0);
	return (0);
}

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

void	ft_close_all(int *fd, int *file_in, int *file_out)
{
	close (fd[0]);
	close (fd[1]);
	close(*file_in);
	close(*file_out);
}
