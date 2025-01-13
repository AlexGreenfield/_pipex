/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:34:06 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/13 19:05:27 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_files(char **argv);
int	ft_check_cmd_1(t_cmd *cmd);
int	ft_check_cmd_2(t_cmd *cmd);
int	ft_check_dir(char *cmd);

int	ft_check_files_cmd(t_cmd *cmd, char **argv)
{
	cmd->cmd_1 = ft_strdup(argv[2]);
	cmd->cmd_2 = ft_strdup(argv[3]);
	ft_check_files(argv);
	if (*cmd->cmd_1 == '\0')
		write(2, "permission denied\n", 19);
	else if (ft_check_dir(cmd->cmd_1) == SUCCESS)
	{
		if (access(cmd->cmd_1, X_OK) != SUCCESS)
			perror(argv[2]);
	}
	else if (ft_check_cmd_1(cmd) != SUCCESS)
		perror(argv[2]);
	if (*cmd->cmd_2 == '\0')
		write(2, "permission denied\n", 19);
	else if (ft_check_dir(cmd->cmd_2) == 0)
	{
		if (access(cmd->cmd_2, X_OK) != SUCCESS)
			perror(argv[3]);
	}
	else if (ft_check_cmd_2(cmd) != SUCCESS)
		perror (argv[3]);
	return (SUCCESS);
}

int	ft_check_files(char **argv)
{
	if (access(argv[1], R_OK) != 0)
		perror(argv[1]);
	return (SUCCESS);
}

int	ft_check_dir(char *cmd)
{
	if (ft_strncmp(cmd, "/", 1) == SUCCESS)
		return (SUCCESS);
	if (ft_strncmp(cmd, "./", 2) == SUCCESS)
		return (SUCCESS);
	if (ft_strncmp(cmd, "../", 3) == SUCCESS)
		return (SUCCESS);
	else
	{
		free(cmd);
		return (FILE_ERROR);
	}
}

int	ft_check_cmd_1(t_cmd *cmd)
{
	int		i;
	char	*temp;

	i = 0;
	while (cmd->cmd_paths[i] != NULL)
	{
		temp = ft_strjoin(cmd->cmd_paths[i], "/");
		cmd->cmd_1 = ft_strjoin(temp, cmd->cmd_arg[0][0]);
		free(temp);
		if (access(cmd->cmd_1, X_OK) == 0)
			return (SUCCESS);
		if (!cmd->cmd_1)
			return (MALLOC_ERROR);
		else
		{
			free(cmd->cmd_1);
			cmd->cmd_1 = NULL;
			i++;
		}
	}
	return (FILE_ERROR);
}

int	ft_check_cmd_2(t_cmd *cmd)
{
	int		i;
	char	*temp;

	i = 0;
	while (cmd->cmd_paths[i] != NULL)
	{
		temp = ft_strjoin(cmd->cmd_paths[i], "/");
		cmd->cmd_2 = ft_strjoin(temp, cmd->cmd_arg[1][0]);
		free(temp);
		if (access(cmd->cmd_2, X_OK) == 0)
			return (SUCCESS);
		if (!cmd->cmd_2)
			return (MALLOC_ERROR);
		else
		{
			free(cmd->cmd_2);
			cmd->cmd_2 = NULL;
			i++;
		}
	}
	return (FILE_ERROR);
}
