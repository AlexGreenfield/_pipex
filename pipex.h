/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 18:24:11 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/23 18:27:24 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Global def
#ifndef PIPEX_H
# define PIPEX_H
#endif

// Libs
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"

// Macros for error
# ifndef SUCCESS
#  define SUCCESS 0
# ifndef MALLOC_ERROR
#  define MALLOC_ERROR 1
# endif

// Structs
typedef struct s_fd_pipe
{
	int	in_fd;
	int	out_fd;
	int	fd[2];
}	t_fd_pipe;

typedef struct s_cmd
{
	char	**cmd_paths;
	char	***cmd_arg;
	int		cmd_count;
}	t_cmd;


// Main pipex
int		ft_pipex(char **argv, t_cmd cmd, t_fd_pipe fd_pipe);

// Args
int ft_parse_args(t_cmd *cmd, int argc, char **argv);

// Check ft
int		ft_check_pipex(char **argv, int *fd, int *file_in, int *file_out);
int		ft_check_fork(int pid, int *fd, int *file_in, int *file_out);

// Close ft
void	ft_close_all(int *fd, int *file_in, int *file_out);

#endif
