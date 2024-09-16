#include "../minishell.h"

void	handler_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_global_exit = 1;
	}
}

void	handler_heredoc(int status)
{
	if (status == SIGINT)
	{
		g_global_exit = 999;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	set_signal(int c)
{
	if (c == MAIN_P)
	{
		signal(SIGINT, &handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (c == CHILD_P)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (c == HEREDOC_P)
	{
		signal(SIGINT, &handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}