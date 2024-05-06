/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:45:11 by istasheu          #+#    #+#             */
/*   Updated: 2024/05/06 14:45:16 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_handler(int signum)
{
	static int	bit_position;
	static int	byte;

	if (signum == SIGUSR1)
	{
		byte |= (1 << bit_position);
	}
	bit_position++;
	if (bit_position == BITS_IN_BYTE)
	{
		ft_printf("%c", byte);
		bit_position = 0;
		byte = 0;
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	(void)argv;
	if (argc != 1)
	{
		ft_printf("Error\n");
		return (0);
	}
	pid = getpid();
	ft_printf("%sPID:%s %d\n", COLOR_BLUE, COLOR_GREEN, pid);
	ft_printf("%sWaiting for client "
		"to send a message...%s\n", COLOR_GRAY, COLOR_RESET);
	while (argc == 1)
	{
		signal(SIGUSR1, ft_handler);
		signal(SIGUSR2, ft_handler);
		pause ();
	}
	return (0);
}
