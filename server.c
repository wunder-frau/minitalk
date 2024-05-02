#include "minitalk.h"

void ft_handler(int signum)
{
	static int	bit_position;
	static int	byte;
	int			BITS_IN_BYTE;
	
	bit_position = 0;
	byte = 0;
	BITS_IN_BYTE = 8;
	if (signum == SIGUSR1) {
		byte |= (1 << bit_position);
		printf("Received SIGUSR1!\n");
	}
	bit_position++;
	if (bit_position == BITS_IN_BYTE) {
		printf("%c", byte);
		bit_position = 0;
		byte = 0;
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	(void)argv;
	if (argc != 1)
	{
		printf("Error\n");
		return (0);
	}
	pid = getpid();
	printf("\033[94mPID\033[0m \033[96m->\033[0m %d\n", pid);
	while (argc == 1)
	{
		signal(SIGUSR1, ft_handler);
		signal(SIGUSR2, ft_handler);
		pause ();
	}
	return (0);
}