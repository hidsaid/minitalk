#include "minitalk.h"

void	handle(int sig, siginfo_t *info, void *oldact)
{
	static int		(bit) = 7,c = 0;
	static pid_t	latest_pid = 0;

	(void)oldact;
	if(latest_pid == 0)
		latest_pid = info->si_pid;
	if (latest_pid != info->si_pid)
	{
		if (kill(latest_pid, 0) != -1)
			return ((void)kill(info->si_pid, SIGUSR2));
		write(1, "\n", 1);
		latest_pid = info->si_pid;
		c = 0;
		bit = 7;
	}
	if (sig == SIGUSR2)
		c = c | (128 >> bit);
	bit--;
	if (bit == -1)
	{
		write(1, &c, 1);
		bit = 7;
		c = 0;
	}
	kill(latest_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handle;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	/*Ensures no signals are blocked during signal handling.
	empty the set of mask signals,
		mask might contain random signals or garbage values that might cause inpredictible behavior
	Leading to unexpected signals being blocked.
	Always use sigemptyset() unless you want to block specific signals.*/
	printf("PID is %d\n", getpid());
	while (1)
		pause();
	return (0);
}
