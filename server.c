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
	ft_putstr_fd(COLOR_RED "Error" COLOR_RESET "\n", 2);
	ft_putstr_fd("\n" COLOR_RED "Server: "
		"unexpected error occurred" COLOR_RESET "\n", 2);
	ft_printf("%sContext: %s%s\n", COLOR_RED, context_message, COLOR_RESET);
	exit(EXIT_FAILURE);
}

static void	handle_byte(unsigned char cur_chr, siginfo_t *info)
{
	if (cur_chr == '\0')
	{
		ft_putchar_fd('\n', 1);
		kill(info->si_pid, SIGUSR1);
	}
	else
	{
		ft_printf(COLOR_GREEN);
		ft_putchar_fd(cur_chr, 1);
		ft_printf(COLOR_RESET);
	}
}

static void	handle_received_signal(int signal, siginfo_t *info, void *context)
{
	static unsigned char	cur_chr;
	static size_t			count;
	int						received_bit;

	if (cur_chr == 0 && count == 0)
	{
		cur_chr = 0;
		count = 8;
	}
	(void)context;
	if (signal == SIGUSR2)
		received_bit = 0;
	else if (signal == SIGUSR1)
		received_bit = 1;
	else
		return ;
	count--;
	cur_chr |= (received_bit << count);
	if (count == 0)
	{
		handle_byte(cur_chr, info);
		cur_chr = 0;
		count = 8;
	}
}

int	main(void)
{
	int					pid;
	struct sigaction	act;

	pid = getpid();
	ft_printf(COLOR_BLUE "Server PID: %d\n", pid);
	// ft_printf("%sWaiting for client "
	// 	"to send a message...%s\n", COLOR_GRAY, COLOR_GREEN);
	ft_memset(&act, '\0', sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = handle_received_signal;
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
