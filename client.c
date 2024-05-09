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
static void	ft_handler(int signum)
{
	(void)signum;
	ft_putendl_fd("Signal Received !", 1);
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
	if (kill(pid, 0) == 0)
	{
		ft_printf(COLOR_GREEN "PID %d is valid. "
			"Message has been sent." COLOR_RESET "\n", pid);
			exit(1);		
	}
	else
		ft_printf(COLOR_RED "PID %d is invalid or refers"
			"to a non-existing process." COLOR_RESET "\n", pid);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        ft_putendl_fd("Usage: <program> <pid> <text_to_send>", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    pid_t target_pid = ft_atoi(argv[1]);
    if (target_pid <= 0) {
        ft_putendl_fd("Invalid process ID provided.", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    if (signal(SIGUSR1, ft_handler) == SIG_ERR) {
        handle_error("Failed to ignore SIGUSR1 signal.");
        return EXIT_FAILURE;
    }
    send_text_as_signals(argv[2], target_pid);
   	check_and_print_pid_status(target_pid);
    pause();
    return (0);
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
