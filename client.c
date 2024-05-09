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

void	handle_client_error(const char *context_message)
{
	ft_putstr_fd(COLOR_RED "Error" COLOR_RESET "\n", 2);
	ft_putstr_fd("\n" COLOR_RED "Client: unexpected error occurred" COLOR_RESET "\n", 2);
	ft_printf("%sContext: %s%s\n", COLOR_RED, context_message, COLOR_RESET);
	exit(EXIT_FAILURE);
}
static void	handle_signal_and_exit(int signum)
{
	(void)signum;
	ft_putstr_fd(COLOR_CYAN "Signal Received!", 1);
	exit(EXIT_SUCCESS);
}

static void send_text_as_signals(const char *text, pid_t target_pid)
{
		size_t char_index;
		size_t remaining_bits;
		size_t text_length;
		int current_bit;

		char_index = 0;
		text_length = ft_strlen(text);
		while (char_index <= text_length)
		{
				remaining_bits = 8;
				while (remaining_bits > 0)
				{
						remaining_bits--;
						current_bit = ((unsigned char)text[char_index] >> remaining_bits) & 1;
						ft_printf("current bit: %d\n", current_bit);
						if (current_bit == 1) {
								kill(target_pid, SIGUSR1);
						} else {
								kill(target_pid, SIGUSR2);
						}
						usleep(100);
				}
				char_index++;
		}
}

void	check_and_print_pid_status(int pid)
{
	ft_printf("pid: %d\n", pid);
	if (kill(pid, 0) == 0)
	{
		ft_printf(COLOR_GREEN "PID %d is valid. "
			"Message has been sent." COLOR_RESET "\n", pid);
		exit(EXIT_SUCCESS);		
	}
	else
	{
		ft_printf(COLOR_RED "PID %d is invalid or refers "
			"to a non-existing process." COLOR_RESET "\n", pid);
			exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char *argv[])
{
	struct sigaction	sa;
	int								pid;

	if (argc != 3)
	{
			handle_client_error("Valid input: <program> <pid> <text_to_send>");
			return EXIT_FAILURE;
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
			handle_client_error("Invalid process ID provided.");
			return EXIT_FAILURE;
	}
	sa.sa_handler = handle_signal_and_exit;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
			handle_client_error("Failed to set SIGUSR1 signal handler.");
			return EXIT_FAILURE;
	}
	send_text_as_signals(argv[2], pid);
	check_and_print_pid_status(pid);
	pause();
	return EXIT_SUCCESS;
}
