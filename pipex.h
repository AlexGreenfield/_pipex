/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 18:24:11 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/18 19:42:21 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"

// Main pipes
int		ft_pipex(char **argv);

// Check ft
int		ft_check_pipex(char **argv, int *fd, int *file_in, int *file_out);
int		ft_check_fork(int pid, int *fd, int *file_in, int *file_out);

// Close ft
void	ft_close_all(int *fd, int *file_in, int *file_out);

#endif
