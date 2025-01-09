/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_libft_other_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 22:44:00 by acastrov          #+#    #+#             */
/*   Updated: 2025/01/08 20:37:28 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	string_size;

	string_size = 0;
	while (s[string_size] != '\0')
		string_size++;
	return (string_size);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	while (*src && size-- > 1)
		*dst++ = *src++;
	*dst = '\0';
	return (src_len);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;

	dst_len = (ft_strlen(dst));
	src_len = (ft_strlen(src));
	if (size <= dst_len)
		return (src_len + size);
	dst += dst_len;
	size -= dst_len;
	while (size-- > 1 && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (dst_len + src_len);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (*s1 && *s2 && i < n && *s1 == *s2)
	{
		s1++;
		s2++;
		i++;
	}
	if (i == n)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join_string;
	size_t	len_join_string;

	if (!s1 || !s2)
		return (NULL);
	len_join_string = ft_strlen(s1) + ft_strlen(s2) + 1;
	join_string = malloc(len_join_string);
	if (!join_string)
		return (NULL);
	ft_strlcpy(join_string, s1, ft_strlen(s1) + 1);
	ft_strlcat(join_string, s2, len_join_string);
	return (join_string);
}
