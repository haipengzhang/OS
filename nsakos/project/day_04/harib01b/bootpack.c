
/*告诉c编译器，有一个函数在别的文件里*/
void io_hlt(void);
void write_mem8(int addr, int data);

void HariMain(void)
{

// fin:
// 	io_hlt();
// 	goto fin;

	int i; /*变量声明：i是一个32位的整数*/
	for (i = 0xa0000; i <= 0xaffff; i++)
	{
		write_mem8(i, 15); /*MOV BYTE [i],15*/
	}

	for (;;) {
		io_hlt();
	}

}