/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 19:24:52 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/03 20:24:17 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_free_cmd(t_cmd *cmd, int flag)
{
	if (cmd->cmd_paths)
		ft_free_cmd_paths(cmd->cmd_paths);
	if (cmd->cmd_arg)
		ft_free_cmd_arg(cmd->cmd_arg);
	if (cmd->cmd_1)
		free(cmd->cmd_1);
	if (cmd->cmd_2)
		free(cmd->cmd_2);
	free(cmd);
	return (flag);
}

void	ft_free_cmd_paths(char **cmd_paths)
{
	int	i;

	i = 0;
	while (cmd_paths[i])
	{
		free(cmd_paths[i]);
		i++;
	}
	free(cmd_paths);
}

void	ft_free_cmd_arg(char ***cmd_arg)
{
	int	i;
	int	j;

	i = 0;
	while (cmd_arg[i])
	{
		j = 0;
		while (cmd_arg[i][j])
		{
			free(cmd_arg[i][j]);
			j++;
		}
		free(cmd_arg[i]);
		i++;
	}
	free(cmd_arg);
}

int	ft_free_fd_pipe(t_fd_pipe *fd_pipe, int flag)
{
	close(fd_pipe->in_fd);
	close(fd_pipe->out_fd);
	close(fd_pipe->fd[0]);
	close(fd_pipe->fd[1]);
	free(fd_pipe);
	return (flag);
}
