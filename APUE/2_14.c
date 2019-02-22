#include "apue.h"
#include <errno.h>
#include <limits.h>

/** 使用
 *	pathconf(int name)
 *	sysconf(const char* pathname, int name) pathname关联的限定值
 *	fpathconf(int fd, int name)获取系统限制
 *  若成功返回相应值，否则-1
 */
static void pr_sysconf(char*, int);
static void pr_pathconf(char*, char const* path, int);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		err_quit("usage: a.out <dirname>");
	}
#ifdef ARG_MAX
	printf("ARG_MAX defined to be %ld\n", (long)ARG_MAX+0);
#else
	printf("no symbol for AGE_MAX\n");
#endif
#ifdef _SC_AGE_MAX
	pr_sysconf("ARG_MAX =", _SC_AGE_MAX);
#else
	printf("no symbol for _SC_AGE_MAX\n");
#endif

#ifdef MAX_CANON
	printf("MAX_CANON defined to be %ld\n", (long)MAX_CANON+0);
#else
	printf("no symbol for MAX_CANON\n");
#endif
#ifdef _PC_MAX_CANON	//The maximum length of a formatted input line
	pr_pathconf("MAX_CANON =", argv[1], _PC_MAX_CANON);
#else
	printf("no symbol for _PC_MAX_CANON\n");
#endif

// similar processing for all the reset of the sysconf symbols...
	return 0;
}

static void pr_sysconf(char* mesg, int name) {
	long val;
	// 输出到标准输出，相当于打印到屏幕
	fputs(mesg, stdout);
	errno = 0;
	if ((val = sysconf(name)) < 0) {
		if (errno != 0) {
			if (errno == EINVAL) {
				fputs("(not supported)\n", stdout);
			} else {
				err_sys("sysconf error");
			}
		} else {
			fputs("(no limit)\n", stdout);
		}
	} else {
		printf("%ld\n", val);
	}
}

static void pr_pathconf(char* mesg, char const* path, int name) {
	long val;
	fputs(mesg, stdout);
	errno = 0;
	if ((val = pathconf(path, name)) < 0) {
		if (errno != 0) {
			if (errno == EINVAL) {
				fputs("(not supported)\n", stdout);
			} else {
				err_sys("pathconf error");
			}
		} else {
			fputs("(no limit)\n", stdout);
		}
	} else {
		printf("%ld\n", val);
	}
}
