/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:26:51 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/18 17:05:41 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_close_all(int *fd, int *file_in, int *file_out)
{
	close (fd[0]);
	close (fd[1]);
	close(*file_in);
	close(*file_out);
}
