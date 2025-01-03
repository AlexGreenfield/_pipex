/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 18:24:11 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/03 21:14:47 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Global def
#ifndef PIPEX_H
# define PIPEX_H

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
# define SUCCESS 0
# define MALLOC_ERROR 1
# define FILE_ERROR 2
# define PIPE_ERROR 3
# define FORK_ERROR 4

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
	char	*cmd_1;
	char	*cmd_2;
}	t_cmd;

// Main pipex
int		ft_pipex(char **argv, t_cmd cmd, t_fd_pipe fd_pipe);
int		ft_init_pipex(t_cmd *cmd, int argc, char **argv, char **enpv);
int		ft_child_1(t_cmd *cmd, t_fd_pipe *fd_pipe, char **enpv);
int		ft_child_2(t_cmd *cmd, t_fd_pipe *fd_pipe, char **enpv);

// Args
int		ft_parse_args(t_cmd *cmd, int argc, char **argv);
int		ft_get_path(t_cmd *cmd, char **envp);
char	*ft_find_path(char **envp);

// Free
int		ft_free_cmd(t_cmd *cmd, int flag);
void	ft_free_cmd_paths(char **cmd_paths);
void	ft_free_cmd_arg(char ***cmd_arg);
int		ft_free_fd_pipe(t_fd_pipe *fd_pipe, int flag);

// Check ft
int		ft_check_files_cmd(t_cmd *cmd, char **argv);
int		ft_check_files(char **argv);
int		ft_check_cmd_1(t_cmd *cmd);
int		ft_check_cmd_2(t_cmd *cmd);

// Open
int		ft_open_fd(t_fd_pipe *fd_pipe, int argc, char **argv);

#endif
