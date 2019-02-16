
/*告诉c编译器，有一个函数在别的文件里*/
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
int io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void)
{
	int i; /*变量声明：i是一个32位的整数*/
	char *p;

	//设定8位彩色模式下的调色板(系统默认，这里我们自己指定一个)
	init_palette();

	p = (char *)0xa0000;//显卡内存区域地址
	for (i = 0; i <= 0xffff; i++)
	{
		p[i] = i & 0x0f;
		/**(p+i)可以写成p[i]这种形式*/
	}

	for (;;) {
		io_hlt();
	}

}

void init_palette(void){

	static unsigned char table_rgb[16*3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:亮红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	set_palette(0, 15, table_rgb);
	/*C语言中的static char语句只能用于数据，相当于汇编中的DB指令*/

}

void set_palette(int start, int end, unsigned char *rgb){

	//0x03c8 0x03c9设备号(调色板)port
	int i, eflags;
	eflags = io_load_eflags(); /*记录中断许可标志的值*/
	io_cli();
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++)
	{
		io_out8(0x03c9, rgb[0]/4);
		io_out8(0x03c9, rgb[1]/4);
		io_out8(0x03c9, rgb[2]/4);
		rgb += 3;
	}
	io_store_eflags(eflags); /*复原中断许可标志*/

}
