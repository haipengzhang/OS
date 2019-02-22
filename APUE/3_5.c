#include "apue.h"

#define BUFFSIZE 4096

// copy实现 ./a.out <testA.txt >testRW.txt
// 程序终止时，自动会关闭打开的文件描述符，所以不用处理close
int main() {
	int n;
	char buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write error!");
		}
	}
	if (n < 0) {
		err_sys("read error!");
	}
	exit(0);
}