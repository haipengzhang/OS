#include "apue.h"
#include <errno.h>

int main(int argc, char const *argv[])
{
	// stderr文件描述符：标准错误输出，strerror传入错误类型返回错误信息
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	// errno， errno.h中定义的全局的（|线程共享的）变量，记录出错类型
	errno = ENOENT;	//No such file or directory
	// perror基于errno在标准错误上产生出错信息；
	// argv[0]：函数名，作为出错信息出现在错误string的前面；
	perror(argv[0]);
	return 0;
}