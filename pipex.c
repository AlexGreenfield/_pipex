/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:27:21 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/23 18:32:01 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	t_cmd *cmd;
	//t_fd_pipe *fd_pipe;

	if (argc == 5)
	{
		cmd = malloc(sizeof(t_cmd));
		if(!cmd)
			return (MALLOC_ERROR);
		if(ft_parse_args(cmd, argc, argv) != SUCCESS)
			return (MALLOC_ERROR);
		printf("First argument is :%s\n", cmd->cmd_arg[0][0]);
		printf("And sub argument is :%s\n", cmd->cmd_arg[0][1]);
		printf("Second argument is :%s\n", cmd->cmd_arg[1][0]);
		printf("And sub argument is :%s\n", cmd->cmd_arg[1][1]);
		printf("Third argument is :%s\n", cmd->cmd_arg[2][0]);
		printf("And sub argument is :%s\n", cmd->cmd_arg[2][1]);

		// Parse args
		// 1 st check cmd and files with access
		// If ok, alloc structs
		// Fill structs
		// Call to child 1
		// Call to chil 2
		// Close everything
	}
	else
	{
		write (2, "Not enough arguments", 21);
		return (1);
	}
	return (0);
}

