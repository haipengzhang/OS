
/*告诉c编译器，有一个函数在别的文件里*/
void io_hlt(void);
void write_mem8(int addr, int data);

void HariMain(void)
{

// fin:
// 	io_hlt();
// 	goto fin;

	int i; /*变量声明：i是一个32位的整数*/
	char *p; /*变量p，用于BYTE型地址*/
	for (i = 0xa0000; i <= 0xaffff; i++)
	{
		p = (char *)i; /*代入地址*/
		*p = i & 0x0f;
		/*这可以替代wirte_mem8(i, i&0x0f)*/
	}

	for (;;) {
		io_hlt();
	}

}

