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

void	handle_server_error(const char *context_message)
{
	ft_printf("\n%sServer: unexpected error occurred%s\n", COLOR_RED, COLOR_RESET);
	ft_printf("%sContext: %s%s\n", COLOR_RED, context_message, COLOR_RESET);
	exit(EXIT_FAILURE);
}

static void handle_byte(unsigned char current_char, siginfo_t *info)
{
	if (current_char == '\0') {
			ft_putchar_fd('\n', 1);
			kill(info->si_pid, SIGUSR1);
	} else {
			ft_putchar_fd(current_char, 1);
	}
}

static void	handle_received_signal(int signal, siginfo_t *info, void *context)
{
	static unsigned char current_char = 0;
	static size_t remaining_bits = 8;
	(void)context;
	int received_bit;
	if (signal == SIGUSR2)
		received_bit = 0;
	else if (signal == SIGUSR1)
		received_bit = 1;
	else
		return ;
	remaining_bits--;
	current_char |= (received_bit << remaining_bits);
	if (remaining_bits == 0) {
			handle_byte(current_char, info);
			current_char = 0;
			remaining_bits = 8;
	}
}

int	main(void)
{
	int					pid;
	struct sigaction	act;

	pid = getpid();
	ft_printf("Server PID: %d\n", pid);
	ft_printf(COLOR_BLUE "Server PID: %d\n", pid);
	ft_printf("%sWaiting for client "
		"to send a message...%s\n", COLOR_GRAY, COLOR_RESET);
	ft_memset(&act, '\0', sizeof(act));
	act.sa_sigaction = handle_received_signal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	while (1)
	{
		if (sigaction(SIGUSR1, &act, NULL) < 0
			|| sigaction(SIGUSR2, &act, NULL) < 0)
		{
			handle_server_error("Falied to set SIGUSR1 and SIGUSR2 handlers");
			exit(EXIT_FAILURE);
		}
		pause();
	}
	return (EXIT_FAILURE);
}

// void	ft_handler(int signum)
// {
// 	static int	bit_position;
// 	static int	byte;

// 	if (signum == SIGUSR1)
// 	{
// 		byte |= (1 << bit_position);
// 	}
// 	bit_position++;
// 	if (bit_position == BITS_IN_BYTE)
// 	{
// 		ft_printf("%c", byte);
// 		bit_position = 0;
// 		byte = 0;
// 	}
// }

// int	main(int argc, char **argv)
// {
// 	int	pid;

// 	(void)argv;
// 	if (argc != 1)
// 	{
// 		ft_printf("Error\n");
// 		return (0);
// 	}
// 	pid = getpid();
// 	ft_printf("%sPID:%s %d\n", COLOR_BLUE, COLOR_GREEN, pid);
// 	ft_printf("%sWaiting for client "
// 		"to send a message...%s\n", COLOR_GRAY, COLOR_RESET);
// 	while (argc == 1)
// 	{
// 		signal(SIGUSR1, ft_handler);
// 		signal(SIGUSR2, ft_handler);
// 		pause ();
// 	}
// 	return (0);
// }
