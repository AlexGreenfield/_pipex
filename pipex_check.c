/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:34:06 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/03 21:14:32 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_files(char **argv);
int	ft_check_cmd_1(t_cmd *cmd);
int	ft_check_cmd_2(t_cmd *cmd);

int	ft_check_files_cmd(t_cmd *cmd, char **argv)
{
	if (ft_check_files(argv) != SUCCESS)
		return (FILE_ERROR);
	if (ft_check_cmd_1(cmd) || ft_check_cmd_2(cmd) != SUCCESS)
		return (FILE_ERROR);
	return (SUCCESS);
}

int	ft_check_files(char **argv)
{
	if (access(argv[1], R_OK) != 0)
	{
		perror("Infile it's not accesible");
		return (FILE_ERROR);
	}
	/*if (access(argv[argc - 1], W_OK) != 0)
	{
		perror("Outfile it's not accesible");
		return (FILE_ERROR);
	}*/
	return (SUCCESS);
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
		if (!cmd->cmd_1)
			return (MALLOC_ERROR);
		if (access(cmd->cmd_1, X_OK) == 0)
		{
			free(temp);
			return (SUCCESS);
		}
		else
		{
			free(cmd->cmd_1);
			cmd->cmd_1 = NULL;
			free (temp);
			i++;
		}
	}
	perror("Cmd 1 it's not accesible");
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
		if (!cmd->cmd_2)
			return (MALLOC_ERROR);
		if (access(cmd->cmd_2, X_OK) == 0)
		{
			free(temp);
			return (SUCCESS);
		}
		else
		{
			free(cmd->cmd_2);
			cmd->cmd_2 = NULL;
			free (temp);
			i++;
		}
	}
	perror("Cmd 2 it's not accesible");
	return (FILE_ERROR);
}
