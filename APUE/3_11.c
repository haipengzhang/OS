#include "apue.h"
#include <fcntl.h>

//第一个参数指定文件描述符，程序对于改文件描述符打印其所选择的文件标志说明
int main(int argc, char const *argv[]) {
	int val;
	if (argc != 2) {
		err_quit("Usage:a.out <descriptor#>");
	}

	//F_GETFL对应与文件状态标志（作为返回值）atoi字符串转换成整型数的一个函数
	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
		err_sys("fcntl error for fd %d", atoi(argv[1]));
	}

	//O_ACCMODE取得访问方式位
	switch (val & O_ACCMODE) {
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;
		default:
			err_dump("unknown access mode");
	}

	if (val & O_APPEND) {
		printf(", append");
	}
	if (val & O_NONBLOCK) {
		printf(", nonblocking");
	}
	if (val & O_SYNC) {
		printf(", synchronous writes");
	}

	#if !defined(_POSIX_C_SOUCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
		if (val & O_FSYNC) {
			printf(", synchronous writes");
		}
	#endif
		putchar('\n');
	return 0;
}

// 参考shell输出重定向
// ./a.out 0 0</dev/tty 0标准输入重定向到/dev/tty文件，用来获取标准输入的文件状态信息
// ./a.out 1 1>temp.foo 1标准输出重定向到temp.foo，获取标准输出的状态信息
// ./a.out 2 2>>temp.foo 2标准error输入重定向到temp.foo，获取标准error输入的状态信息
// ./a.out 5 5<>temp.foo 表示在文件描述5上打开文件temp.foo以供读写


