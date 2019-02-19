#include "apue.h"
#define BUFFSIZE 4096

/**
 * 文件描述符小的非负整数，用以标示一个特定进程正在访问的文件；
 * 当打开一个文件时候都返回一个文件描述符，用以读写
 * STDIN_FILENO与STDOUT_FILENO是标准输入和标准输出文件描述符，shell默认打开
 * 不带缓冲的IO函数read/write
 */
int main() {
	int n;
	char buf[BUFFSIZE];
	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write error");
		}
	}
	if (n < 0) {
		err_sys("read error");
	}
	exit(0);
}