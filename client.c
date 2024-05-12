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

static void	client_error(const char *context_message)
{
	ft_putstr_fd("\n" COLOR_RED "Client: error" COLOR_RESET "\n", 2);
	ft_printf(COLOR_BLUE);
	ft_printf("Context: %s\n", context_message);
	exit(EXIT_FAILURE);
}

static void	handle_signal_and_exit(int signal)
{
	if (signal == SIGUSR1)
		ft_putstr_fd(COLOR_GREEN "\n" "Message sent successfully.\n", 1);
	exit(EXIT_SUCCESS);
}

static void	send_text_as_signals(const char *text, pid_t pid)
{
	size_t	i;
	size_t	len;
	size_t	count;
	int			current_bit;

	i = 0;
	len = ft_strlen(text);
	while (i <= len)
	{
		count = 8;
		while (count > 0)
		{
			count--;
			current_bit = ((unsigned char)text[i] >> count) & 1;
			if (current_bit == 1)
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			usleep(100);
		}
		i++;
	}
}

static void	check_pid_status(int pid)
{
	if (kill(pid, 0) != 0)
	{
		ft_putstr_fd("\n", 2);
		ft_printf(COLOR_RED);
		ft_putstr_fd("Error", 2);
		ft_putstr_fd("\n", 2);
		ft_printf(COLOR_RED "PID %d is invalid or refers "
			"to a non-existing process." COLOR_RESET "\n", pid);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char *argv[])
{
	struct sigaction	sa;
	int					pid;

	if (argc != 3)
	{
		client_error("Valid input: <program> <pid> <text_to_send>");
		return (EXIT_FAILURE);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		client_error("Invalid process ID provided.");
		return (EXIT_FAILURE);
	}
	sa.sa_handler = handle_signal_and_exit;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		client_error("Failed to set SIGUSR1 signal handler.");
		return (EXIT_FAILURE);
	}
	send_text_as_signals(argv[2], pid);
	check_pid_status(pid);
	pause();
	return (0);
}
