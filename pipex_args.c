/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:23:07 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/23 18:23:10 by acastrov         ###   ########.fr       */
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
	cmd->cmd_arg = malloc(cmd->cmd_count * sizeof(char **));
	if(!cmd->cmd_arg)
		return (MALLOC_ERROR);
	while (i < cmd->cmd_count)
	{
		cmd->cmd_arg[i] = ft_split(argv[i + 2], ' ');
		if (!cmd->cmd_arg[i])
		{
			while (j <= i)
			{
				free(cmd->cmd_arg[j]);
				j++;
			}
			free(cmd->cmd_arg);
			return (MALLOC_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
