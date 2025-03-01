#include "minitalk.h"

volatile sig_atomic_t	flag = 0;

void	handler(int sig)
{
	if(sig == SIGUSR1)
		flag = 1;
	else
	{
		printf("sERVER IS BUSY!");
		exit(1);
	}
}

void	send_string(int pid_server, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (j < 8)
		{
			if (((str[i] >> j) & 1) == 0)
				kill(pid_server, SIGUSR1);
			else
				kill(pid_server, SIGUSR2);
			while (flag == 0)
				;
			flag = 0;
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	if (argc != 3)
		return (0);
	int pid_server = ft_atoi(argv[1]);
	send_string(pid_server, argv[2]);
	return (0);
}