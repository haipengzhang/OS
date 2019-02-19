#include "apue.h"
#include <sys/wait.h>

// 捕获信号
static void sig_int(int); // our signal-catching function

int main(int argc, char const *argv[])
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	// 把SIGINT信号（终端信号）和捕获函数sig_int绑定
	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err_sys("signal error");
	}

	printf("%% ");
	// fgets从标准输入读取一行
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = 0;
		}
		// fock()调用新的子进程，新进程是自己的副本
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {
			// execlp要求buf以null结尾所以用fgets读取, execlp重置子进程的程序文件
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}

		// parent
		if ((pid = waitpid(pid, &status, 0)) < 0) {
			err_sys("waitpid error");
		}
		printf("%% ");
	}

	return 0;
}

static void sig_int(int signo)
{
	printf("interupt\n%%");
}