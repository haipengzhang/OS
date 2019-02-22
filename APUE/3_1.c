#include "apue.h"

// 判断标准输入能不能seek
int main() {
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
		printf("cannot seek\n");
	} else {
		printf("seek ok\n");
	}
	exit(0);
}