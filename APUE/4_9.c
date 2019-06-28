#include "apue.h"
#include <fcntl.h>

#define RERERE(S_IRUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

// umask为1的位文件的mode中相应的位就一定被关闭
int main(int argc, char const *argv[])
{
	umask(0);
	if (creat("foo", RWRWRW) < 0) {
		err_sys("creat error for foo");
	}
	umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	if (creat("bar", RWRWRW) < 0) {
		err_sys("creat error for bar");
	}
	return 0;
}