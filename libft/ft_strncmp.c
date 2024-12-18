/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:14:09 by acastrov          #+#    #+#             */
/*   Updated: 2024/10/01 16:45:00 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// Compares 2 strings and gives char ASCII value diff, 0 if there are equal
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
