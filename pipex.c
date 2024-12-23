/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:27:21 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/23 20:31:44 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		ft_free_cmd(cmd, FILE_ERROR);
	}
	else
	{
		write(2, "Not enough arguments", 21);
		return (1);
	}
	return (0);
}
