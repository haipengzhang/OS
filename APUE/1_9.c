#include "apue.h"

// getuid获取用户id，getgid获取该用户的组id，一个用户可能有多个组id
int main(int argc, char const *argv[])
{
	printf("uid = %d, gid = %d\n", getuid(), getgid());
	exit(0);
}