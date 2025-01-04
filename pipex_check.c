/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:34:06 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/04 21:40:30 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_files(char **argv);
int	ft_check_cmd_1(t_cmd *cmd);
int	ft_check_cmd_2(t_cmd *cmd);

int	ft_check_files_cmd(t_cmd *cmd, char **argv)
{
	cmd->cmd_1 = ft_strdup(argv[2]);
	cmd->cmd_2 = ft_strdup(argv[3]);

	printf("Hola por aqui, cmd_1 es :%s\n", cmd->cmd_1);
	if (ft_check_files(argv) != SUCCESS)
		return (FILE_ERROR);
	if (ft_strncmp(cmd->cmd_1, "/", 1) == 0 || ft_strncmp(cmd->cmd_1, "./", 2) == 0 || ft_strncmp(cmd->cmd_1, "../", 3) == 0) // This is the way
	{
		if (access(cmd->cmd_1, X_OK) != SUCCESS)
		{
			printf("Hola en ./, cmd_1 es :%s\n", cmd->cmd_1);
			return (FILE_ERROR);
		}
	}
	else if (ft_check_cmd_1(cmd) != SUCCESS) // Check here absolute path, cmd1 it's not assigned
	{
		printf("Hola por chek, cmd_1 es :%s\n", cmd->cmd_1);
		return (FILE_ERROR);
	}
	if (access(cmd->cmd_2, X_OK) != SUCCESS && ft_check_cmd_2(cmd) != SUCCESS) // Same, cmd2 not assigned
		return (FILE_ERROR);
	return (SUCCESS);
}

int	ft_check_files(char **argv)
{
	if (access(argv[1], R_OK) != 0)
		write(2, "Infile it's not accesible\n", 27);
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
		printf("Hola por aqui, cmd_1 es :%s\n", cmd->cmd_1);
		if (access(cmd->cmd_1, X_OK) == 0)
		{
			printf("Cmd es ejecutable? :%s\n", cmd->cmd_1);
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
	perror("Cmd 1 it's not accesible\n");
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
	perror("Cmd 2 it's not accesible\n");
	return (FILE_ERROR);
}
