#include "apue.h"

// getpid() 返回一个pid_t类型
int main(int argc, char const *argv[])
{
	printf("hello world from process ID %ld\n", (long)getpid());
	exit(0);
}