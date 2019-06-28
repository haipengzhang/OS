#include "apue.h"
#include <fcntl.h>

// 文件的有效用户id可以设置，文件的open权限取决于文件的有效用户；
// 有效用户大部分等于实际用户（登陆用户）,但是也是可以通过setid设置；
// access()以及faccess()函数测试实际用户访问权限；
int main(int argc, char const *argv[]) {
	if (argc != 2) {
		err_quit("usage: a.out <pathname>");
	}
	if (access(argv[1], R_OK) < 0) {
		err_ret("access error for %s", argv[1]);
	} else {
		printf("read access OK\n");
	}

	if (open(argv[1], O_RDONLY) < 0) {
		err_ret("open error for %s", argv[1]);
	} else {
		printf("open for reading OK\n");
	}
	return 0;
}