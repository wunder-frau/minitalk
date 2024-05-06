/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:45:00 by istasheu          #+#    #+#             */
/*   Updated: 2024/05/06 14:45:02 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_send_bits(int pid, char i)
{
	int	bit_position;

	bit_position = 0;
	while (bit_position < BITS_IN_BYTE)
	{
		if ((i & (1 << bit_position)) != 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(100);
		bit_position++;
	}
}

void	check_and_print_pid_status(int pid)
{
	if (kill(pid, 0) == 0)
		ft_printf(COLOR_GREEN "PID %d is valid. "
			"Message has been sent." COLOR_RESET "\n", pid);
	else
		ft_printf(COLOR_RED "PID %d is invalid or refers"
			"to a non-existing process." COLOR_RESET "\n", pid);
}

int	main(int argc, char **argv)
{
	int	pid;
	int	i;

	i = 0;
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		while (argv[2][i] != '\0')
		{
			ft_send_bits(pid, argv[2][i]);
			i++;
		}
		ft_send_bits(pid, '\n');
	}
	else
	{
		ft_printf(COLOR_RED "Error: Please provide"
			"a valid PID and message." COLOR_RED "\n");
		ft_printf(COLOR_CYAN "Valid input: ./client <PID> "
			"<MESSAGE>" COLOR_CYAN "\n");
		return (1);
	}
	check_and_print_pid_status(pid);
	return (0);
}
