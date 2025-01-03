/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_libft_other_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 22:44:00 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/03 23:08:23 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strdup(const char *s)
{
	char	*new_string;
	size_t	sl;

	sl = ft_strlen(s) + 1;
	new_string = malloc(sl);
	if (new_string == NULL)
		return (NULL);
	ft_strlcpy(new_string, s, sl);
	return (new_string);
}

char	*ft_strchr(const char *s, int c)
{
	char	*c_find_pointer;

	c_find_pointer = (char *)s;
	while (*c_find_pointer)
	{
		if (*c_find_pointer == (char)c)
			return ((char *)c_find_pointer);
		c_find_pointer++;
	}
	if ((char)c == '\0')
		return ((char *)c_find_pointer);
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_string;
	size_t	sub_string_len;
	size_t	i;

	if (!s)
		return (NULL);
	sub_string_len = ft_strlen(s);
	if (start >= sub_string_len)
		return (ft_strdup(""));
	if (len > sub_string_len - start)
		len = sub_string_len - start;
	sub_string = malloc(len + 1);
	if (!sub_string)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub_string[i] = s[i + start];
		i++;
	}
	sub_string[i] = '\0';
	return (sub_string);
}
