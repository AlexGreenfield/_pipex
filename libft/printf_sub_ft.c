/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_sub_ft.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:33:00 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/17 17:24:13 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Writes a given string
int	ft_putstr_printf(char *s)
{
	if (!s)
		return ((int)write(1, "(null)", 6));
	if (*s)
		return ((int)write(1, s, ft_strlen(s)));
	return (0);
}

// Writes an int
int	ft_putnbr_printf(int n)
{
	int	d;
	int	i;

	if (n == -2147483648)
		return ((int)write(1, "-2147483648", 11));
	i = 0;
	if (n < 0)
	{
		i += (int)write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
		i += ft_putnbr_printf(n / 10);
	d = (n % 10) + '0';
	i += (int)write(1, &d, 1);
	return (i);
}

// Writes an unsigned int
int	ft_putuint_printf(unsigned int n)
{
	unsigned int	d;
	unsigned int	i;

	i = 0;
	if (n >= 10)
		i += ft_putnbr_printf(n / 10);
	d = (n % 10) + '0';
	i += (unsigned int)write(1, &d, 1);
	return (i);
}

// Writes a hex decimal number
int	ft_hex(unsigned int n, char specifier)
{
	int			i;
	char		*hex_base;

	if (specifier == 'X')
		hex_base = "0123456789ABCDEF";
	else
		hex_base = "0123456789abcdef";
	i = 0;
	if (n >= 16)
		i += ft_hex(n / 16, specifier);
	i += (int)write(1, &hex_base[n % 16], 1);
	return (i);
}

// Writes a pointer address
int	ft_ptr(uintptr_t ptr, int flag)
{
	int		i;
	char	*hex_base;

	if (!ptr)
		return ((int)write(1, "(nil)", 5));
	i = 0;
	hex_base = "0123456789abcdef";
	if (flag == 1)
		i += (int)write(1, "0x", 2);
	if (ptr > 16)
		i += ft_ptr(ptr / 16, 0);
	i += (int)write(1, &hex_base[ptr % 16], 1);
	return (i);
}
