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

static void	server_error(const char *context_message)
{
	ft_putstr_fd("\n" COLOR_RED "Server: error" COLOR_RESET "\n", 2);
	ft_printf(COLOR_BLUE);
	ft_printf("Context: %s\n", context_message);
	exit(EXIT_FAILURE);
}

static void	handle_received_signal(int signal, siginfo_t *info, void *context)
{
	static unsigned char	cur_chr;
	static size_t			count;

	if (cur_chr == 0 && count == 0)
	{
		count = 8;
	}
	(void)context;
	if (signal == SIGUSR2)
		signal = 0;
	else if (signal == SIGUSR1)
		signal = 1;
	count--;
	cur_chr |= (signal << count);
	if (count == 0)
	{
		if (!cur_chr)
		{
			ft_putchar_fd('\n', 1);
			kill(info->si_pid, SIGUSR1);
		}
		ft_putchar_fd(cur_chr, 1);
		cur_chr = 0;
		count = 8;
	}
}

int	main(void)
{
	int					pid;
	struct sigaction	act;

	pid = getpid();
	ft_printf(COLOR_BLUE "Server PID: %d\n" COLOR_RESET, pid);
	ft_printf(COLOR_GRAY "Please enter: <./client> "
		"<pid> <text_to_send> \n" COLOR_RESET);
	ft_memset(&act, '\0', sizeof(act));
	act.sa_sigaction = handle_received_signal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	while (1)
	{
		if (sigaction(SIGUSR1, &act, NULL) < 0
			|| sigaction(SIGUSR2, &act, NULL) < 0)
		{
			server_error("Falied to set SIGUSR1 and SIGUSR2 handlers");
			exit(EXIT_FAILURE);
		}
		pause();
	}
	return (0);
}
