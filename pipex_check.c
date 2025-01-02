/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:34:06 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/02 19:45:31 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_files_cmd(t_cmd *cmd, int argc, char **argv)
{
	if (access(argv[1], R_OK) != 0)
	{
		perror("Infile it's not accesible");
		return (FILE_ERROR);
	}
	if (access(argv[argc - 1], W_OK) != 0)
	{
		perror("Outfile it's not accesible");
		return (FILE_ERROR);
	}
	if (access(cmd->cmd_arg[0][0], X_OK) != 0)
	{
		perror("Cmd 1 it's not accesible");
		return (FILE_ERROR);
	}
	if (access(cmd->cmd_arg[1][0], X_OK) != 0)
	{
		perror("Cmd 2 it's not accesible");
		return (FILE_ERROR);
	}
	return (SUCCESS);
}
