#include "apue.h"
#include <fcntl.h>

// 修改文件标志值（注意直接用F_SETFD|F_SETFL会关闭以前设置的标志位）

void set_fl(int fd, int flags) /* flags are file status flags to turn on*/
{
	int val;
	if ((val = fcntl(flags, F_GETFL, 0)) < 0) {
		err_sys("fcntl error for fd %d", flags);
	}

	val |= flags;	// 打开 如果是val &= ~flags;就是关闭
	if (fcntl(fd, F_SETFL, val) < 0) {
		err_sys("fcntl F_SETFL error");
	}
}

// ioctl函数对文件的特殊操作（磁盘倒带类似这种）