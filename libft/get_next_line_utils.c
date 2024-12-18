/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:30:22 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/17 17:32:31 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// Calculates size of str
size_t	ft_strlen(const char *s)
{
	size_t	string_size;

	if (!s)
		return (0);
	string_size = 0;
	while (s[string_size] != '\0')
		string_size++;
	return (string_size);
}

// Searchs for n and returns pointer
char	*ft_strchr_n(const char *s)
{
	char	*c_find_pointer;

	if (!s)
		return (NULL);
	c_find_pointer = (char *)s;
	while (*c_find_pointer)
	{
		if (*c_find_pointer == '\n')
			return ((char *)c_find_pointer);
		c_find_pointer++;
	}
	return (NULL);
}

// Copies a const string on a new string with malloc
char	*ft_strdup(const char *s)
{
	char	*new_string;
	size_t	sl;
	size_t	i;

	if (!s)
		return (NULL);
	sl = ft_strlen(s);
	new_string = malloc(sl + 1);
	if (new_string == NULL)
		return (NULL);
	i = 0;
	while (i < sl)
	{
		new_string[i] = s[i];
		i++;
	}
	new_string[i] = '\0';
	return (new_string);
}

// Join two different strings on a new one via malloc
/*char	*ft_strjoin(char *s1, char *s2)
{
	char	*join_string;
	size_t	len_join_string;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len_join_string = ft_strlen(s1) + ft_strlen(s2) + 1;
	join_string = malloc(len_join_string);
	if (!join_string)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		join_string[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		join_string[i++] = s2[j++];
	join_string[i] = '\0';
	return (join_string);
}*/

// Creates a substring of s from start position to n len bytes
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
