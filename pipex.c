/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:27:21 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/03 21:15:10 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_init_pipex(t_cmd *cmd, int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;

	if (argc == 5)
	{
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (MALLOC_ERROR);
		if (ft_parse_args(cmd, argc, argv) != SUCCESS)
			return (ft_free_cmd(cmd, MALLOC_ERROR));
		if (ft_get_path(cmd, envp) != SUCCESS)
			return (ft_free_cmd(cmd, FILE_ERROR));
		if (ft_check_files_cmd(cmd, argv) != SUCCESS)
			return (ft_free_cmd(cmd, FILE_ERROR));
		if (ft_init_pipex(cmd, argc, argv, envp) != SUCCESS)
			return (ft_free_cmd(cmd, FILE_ERROR));
		return (ft_free_cmd(cmd, SUCCESS));
	}
	else
	{
		write(2, "Not enough arguments", 21);
		return (1);
	}
	return (0);
}

int	ft_init_pipex(t_cmd *cmd, int argc, char **argv, char **enpv)
{
	t_fd_pipe	*fd_pipe;
	int			pid1;
	int			pid2;

	fd_pipe = malloc(sizeof(t_fd_pipe));
	if (!fd_pipe)
		return (MALLOC_ERROR);
	if (ft_open_fd(fd_pipe, argc, argv) != SUCCESS)
		return (ft_free_fd_pipe(fd_pipe, FILE_ERROR));
	pid1 = fork();
	if (pid1 < 0)
		return (ft_free_fd_pipe (fd_pipe, FORK_ERROR));
	if (pid1 == 0)
		ft_child_1(cmd, fd_pipe, enpv);
	pid2 = fork ();
	if (pid2 < 0)
		return (ft_free_fd_pipe (fd_pipe, FORK_ERROR));
	if (pid2 == 0)
		ft_child_2(cmd, fd_pipe, enpv);
	ft_free_fd_pipe(fd_pipe, SUCCESS);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (SUCCESS);
}

int	ft_child_1(t_cmd *cmd, t_fd_pipe *fd_pipe, char **enpv)
{
	if (dup2(fd_pipe->in_fd, STDIN_FILENO) < 0)
	{
		perror("Error duplicating file_in");
		exit(1);
	}
	if (dup2(fd_pipe->fd[1], STDOUT_FILENO) < 0)
	{
		perror("Error duplicating pipe write end");
		exit(1);
	}
	ft_free_fd_pipe(fd_pipe, SUCCESS);
	execve(cmd->cmd_1, cmd->cmd_arg[0], enpv);
	perror("Error executing cmd1");
	exit(1);
	return (FORK_ERROR);
}

int	ft_child_2(t_cmd *cmd, t_fd_pipe *fd_pipe, char **enpv)
{
	if (dup2(fd_pipe->fd[0], STDIN_FILENO) < 0)
	{
		perror("Error duplicating pipe write beggining");
		exit(1);
	}
	if (dup2(fd_pipe->out_fd, STDOUT_FILENO) < 0)
	{
		perror("Error duplicating file_out");
		exit(1);
	}
	ft_free_fd_pipe(fd_pipe, SUCCESS);
	execve(cmd->cmd_2, cmd->cmd_arg[1], enpv);
	perror("Error executing cmd2");
	exit(1);
	return (FORK_ERROR);
}
