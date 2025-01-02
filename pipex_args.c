/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:23:07 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/02 21:08:59 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_parse_args(t_cmd *cmd, int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	cmd->cmd_count = argc - 3;
	cmd->cmd_arg = NULL;
	cmd->cmd_1 = NULL;
	cmd->cmd_2 = NULL;
	cmd->cmd_arg = malloc((cmd->cmd_count + 1) * sizeof(char **));
	if (!cmd->cmd_arg)
		return (MALLOC_ERROR);
	while (i < cmd->cmd_count)
	{
		cmd->cmd_arg[i] = ft_split(argv[i + 2], ' ');
		if (!cmd->cmd_arg[i])
			return (MALLOC_ERROR);
		i++;
	}
	cmd->cmd_arg[i] = NULL;
	return (SUCCESS);
}

int	ft_get_path(t_cmd *cmd, char **envp)
{
	char	*path_envp;

	path_envp = ft_find_path(envp);
	if (path_envp == NULL)
		return (FILE_ERROR);
	cmd->cmd_paths = ft_split(path_envp, ':');
	if (!cmd->cmd_paths)
		return (MALLOC_ERROR);
	return (SUCCESS);
}

char	*ft_find_path(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}
