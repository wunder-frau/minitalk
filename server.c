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
// TO DO remove
void	handle_error(const char *context_message)
{
	ft_printf("\n%sServer: unexpected error occurred%s\n", COLOR_RED, COLOR_RESET);
    ft_printf("%sContext: %s%s\n", COLOR_RED, context_message, COLOR_RESET);
	exit(EXIT_FAILURE);
}


void	handle_byte(char* ch, int* rcount, int* cl_pid, int* bpos)
{
	ft_printf("%c", *ch);
	if (*ch == '\0')
	{
		ft_printf("\n%s%d signal recieved from client PID: %d%s\n",
			COLOR_GREEN, *rcount, *cl_pid, COLOR_RESET);
		*rcount = 0;
		*ch = 0;
		if (kill(*cl_pid, SIGUSR1) == -1)
			handle_error("Failed to send SIGUSR1 signal to client process.");
		*bpos = 0;
		return ;
	}
	*bpos = 0;
}

void	process_client_signal(int signum, siginfo_t *info, void *context)
{
	static int	cl_pid;
	static int	bit;
	static char	ch;
	static int	rcount;
	static int	cur_pid;

	(void)context;
	if (!cl_pid)
		cl_pid = info->si_pid;
	cur_pid = info->si_pid;
	if (cl_pid != cur_pid)
	{
		cl_pid = cur_pid;
		bit = 0;
		ch = 0;
		rcount = 0;
	}
	ch |= (signum == SIGUSR2);
	rcount++;
	bit++;
	if (bit == BITS_IN_BYTE)
		handle_byte(&ch, &rcount, &cl_pid, &bit);
	ch <<= 1;
	usleep(100);
	kill(cl_pid, SIGUSR2);
}

int	main(void)
{
	int					pid;
	struct sigaction	act;

	pid = getpid();
	ft_printf("%sPID:%s %d\n", COLOR_BLUE, COLOR_GREEN, pid);
	ft_printf("%sWaiting for client "
		"to send a message...%s\n", COLOR_GRAY, COLOR_RESET);
	act.sa_sigaction = process_client_signal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	while (1)
	{
		sigaction(SIGUSR1, &act, 0);
		sigaction(SIGUSR2, &act, 0);
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
