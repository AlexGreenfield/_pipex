/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:31:21 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/18 18:44:10 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	if (argc == 5)
	{
		if (ft_pipex(argv) == 0)
			printf("Exito en pipex\n");
	}
	return (0);
}
