#include "minitalk.h"

void	ft_send_bits(int pid, char i)
{
	int	bit_position;
	int	BITS_IN_BYTE;

	bit_position = 0;
	BITS_IN_BYTE = 8;
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
				printf(COLOR_GREEN "PID %d is valid and refers"
				"to an existing process." COLOR_RESET "\n", pid);
			else
				printf(COLOR_RED "PID %d is invalid or refers"
				"to a non-existing process." COLOR_RESET "\n", pid);
}

int	main(int argc, char **argv)
{
	int	pid;
	int	i;

	i = 0;
	if (argc == 3)
	{
		pid = atoi(argv[1]);
		while (argv[2][i] != '\0')
		{
			ft_send_bits(pid, argv[2][i]);
			i++;
		}
		ft_send_bits(pid, '\n');
	}
	else
	{
		printf(COLOR_RED "Error: Please provide"
		"a valid PID and message." COLOR_RED "\n");
		printf(COLOR_CYAN "Valid input: ./client <PID>"
		"<MESSAGE>" COLOR_CYAN "\n");
		return (1);
	}
	check_and_print_pid_status(pid);
	return (0);
}
