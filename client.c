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

void	handle_error(const char *context_message)
{
	ft_printf("\n%sServer: unexpected error occurred%s\n", COLOR_RED, COLOR_RESET);
	ft_printf("%sContext: %s%s\n", COLOR_RED, context_message, COLOR_RESET);
	exit(EXIT_FAILURE);
}

void	received(int sig, siginfo_t *info, void *context)
{
	static int sent_signals = 0;
	(void)context;
	(void)info;

	if (sig == SIGUSR1) {
		ft_printf("Received SIGUSR1, %d signal(s) sent successfully!\n", ++sent_signals);
		exit(EXIT_SUCCESS);
	}
	else if (sig == SIGUSR2) {
		ft_printf("Received SIGUSR2, %d signal(s) sent!\n", ++sent_signals);
	}
}

void	send_char_as_binary(unsigned char c, int pid)
{
	int	bit;

	bit = 0;
	while (bit < BITS_IN_BYTE)
	{
		if (c & 128)
		{
			if (kill(pid, SIGUSR2) == -1)
				handle_error("Error sending SIGUSR2");
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
				handle_error("Error sending SIGUSR1");
		}
		c <<= 1;
		bit++;
		pause();
		usleep(100);
	}
}

void	send_text_to_process(char *str, int pid)
{
	int	i;

	i = 0;
	while (str[i])
		send_char_as_binary(str[i++], pid);
	send_char_as_binary('\0', pid);
}

void	handle_received_signal(int sig)
{
	static int	sent;

	if (sig == SIGUSR1)
	{
		ft_printf("%s%d signal sent successfully!%s\n", COLOR_GREEN, ++sent, COLOR_RESET);
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGUSR2)
		++sent;
}

int	main(int ac, char **av)
{
	struct sigaction sa;
	int	server_pid;
	int	client_pid;

	client_pid = getpid();
	if (ac == 3)
	{
		ft_printf("%sclient pid: %d%s\n", COLOR_RED, client_pid, COLOR_RESET);
		sa.sa_sigaction = received;
		sa.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
		server_pid = ft_atoi(av[1]);
		ft_printf("%sText currently sending.. %s\n", COLOR_BLUE, COLOR_RESET);
		send_text_to_process(av[2], server_pid);
	}
	else
		ft_printf("%susage: ./client <server_pid> <text to send>%s\n",
			COLOR_RED, COLOR_RESET);
	return (EXIT_FAILURE);
}


// void	ft_send_bits(int pid, char i)
// {
// 	int	bit_position;

// 	bit_position = 0;
// 	while (bit_position < BITS_IN_BYTE)
// 	{
// 		if ((i & (1 << bit_position)) != 0)
// 			kill(pid, SIGUSR1);
// 		else
// 			kill(pid, SIGUSR2);
// 		usleep(100);
// 		bit_position++;
// 	}
// }

// void	check_and_print_pid_status(int pid)
// {
// 	if (kill(pid, 0) == 0)
// 		ft_printf(COLOR_GREEN "PID %d is valid. "
// 			"Message has been sent." COLOR_RESET "\n", pid);
// 	else
// 		ft_printf(COLOR_RED "PID %d is invalid or refers"
// 			"to a non-existing process." COLOR_RESET "\n", pid);
// }

// int	main(int argc, char **argv)
// {
// 	int	pid;
// 	int	i;

// 	i = 0;
// 	if (argc == 3)
// 	{
// 		pid = ft_atoi(argv[1]);
// 		while (argv[2][i] != '\0')
// 		{
// 			ft_send_bits(pid, argv[2][i]);
// 			i++;
// 		}
// 		ft_send_bits(pid, '\n');
// 	}
// 	else
// 	{
// 		ft_printf(COLOR_RED "Error: Please provide"
// 			"a valid PID and message." COLOR_RED "\n");
// 		ft_printf(COLOR_CYAN "Valid input: ./client <PID> "
// 			"<MESSAGE>" COLOR_CYAN "\n");
// 		return (1);
// 	}
// 	check_and_print_pid_status(pid);
// 	return (0);
// }
