/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:59:58 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/20 17:23:56 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int		ft_init_pipex(char **argv);
int		ft_pipex(char **argv, t_cmd cmd, t_fd_pipe fd_pipe);
static int	child_1(char **argv, int *fd, int *file_in, int *file_out);
static int	child_2(char **argv, int *fd, int *file_in, int *file_out);

int	main(int argc, char **argv)
{
	if (argc == 5)
	{
		//  Init args struct
		// Parse args
		// 1 st check cmd and files with access
		// If ok, alloc structs
		// Fill structs
		// Call to child 1
		// Call to chil 2
		// Close everything

		ft_init_pipex(argv);
	}
	else
	{
		write (2, "Not enough arguments", 21);
		return (1);
	}
	return (0);
}
int	ft_init_pipex_structs(char **argv)
{
	t_cmd *cmd;
	t_fd_pipe *fd_pipe;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		write(2, "Error in malloc cmd", 20);
		return (1);
	}
	fd_pipe = malloc(sizeof(t_fd_pipe));
	if (!fd_pipe)
	{
		write(2, "Error in malloc fd_pipe", 24);
		free(cmd);
		return (1);
	}
	if (ft_fill_fd_pipe (argv, &fd_pipe) != 0)
	{
		write(2, "Error in fill_fd_pipe", 24);
		free(cmd);
		return (1);
	}
}

int	ft_pipex(char **argv, t_cmd cmd, t_fd_pipe fd_pipe)
{

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
