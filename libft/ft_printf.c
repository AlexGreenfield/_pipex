/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastrov <acastrov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:22:41 by acastrov          #+#    #+#             */
/*   Updated: 2024/12/17 17:23:07 by acastrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checker(char const *format);
static int	ft_switcher(char specifier, va_list args);

// Replicates the behavior of printf
int	ft_printf(char const *format, ...)
{
	int		i;
	va_list	args;

	if (ft_checker(format) == 0)
		return (-1);
	va_start(args, format);
	i = 0;
	while (*format)
	{
		if (*format != '%')
		{
			i += write(1, format, 1);
			format++;
		}
		if (*format == '%')
		{
			format++;
			if (!*format)
				break ;
			i += ft_switcher(*format, args);
			format++;
		}
	}
	va_end(args);
	return (i);
}

// Checks if there's format or if there's a invalid specifier
static int	ft_checker(char const *format)
{
	char	*set;

	if (!format)
		return (0);
	set = "cspdiuxX%";
	while (*format)
	{
		while (*format && *format != '%')
			format++;
		if (*format == '%')
		{
			format++;
			if (!*format || ft_strchr(set, *format) == NULL)
			{
				write(2, "error: invalid or incomplete format specifier", 47);
				return (0);
			}
			format++;
		}
	}
	return (1);
}

// Switch ft according to specifier
static int	ft_switcher(char specifier, va_list args)
{
	int	switch_i;
	int	ch;

	switch_i = 0;
	if (specifier == '%')
		return ((int)write(1, "%", 1));
	else if (specifier == 'c')
	{
		ch = va_arg(args, int);
		switch_i = write(1, &ch, 1);
	}
	else if (specifier == 's')
		switch_i = ft_putstr_printf(va_arg(args, char *));
	else if (specifier == 'd' || specifier == 'i')
		switch_i = ft_putnbr_printf(va_arg(args, int));
	else if (specifier == 'u')
		switch_i = ft_putuint_printf(va_arg(args, unsigned int));
	else if (specifier == 'x' || specifier == 'X')
		switch_i = ft_hex(va_arg(args, unsigned int), specifier);
	else if (specifier == 'p')
		switch_i += ft_ptr(va_arg(args, uintptr_t), 1);
	return (switch_i);
}
