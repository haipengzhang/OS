
#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;

void enable_mouse(void);
void init_keyboard(void);

void HariMain(void)
{
	struct  BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32];
	int mx, my, i;

	fifo8_init(&keyfifo, 32, keybuf);

	init_gdtidt();
	init_pic();//设置pic的寄存器
	io_sti();//设置cpu接收外部中断
	// 修改了pic的imr，接收中断
	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);
	
	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	
	enable_mouse();

	for (;;) {
		io_cli();
		if(fifo8_status(&keyfifo) == 0){
			io_stihlt();
		}else{
			i = fifo8_get(&keyfifo);
			io_sti();
			sprintf(s, "%02X", i);
			boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
			putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
		}
	}

}

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/*等待键盘控制电路准备完毕*/
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	/*初始化键盘控制电路*/
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE); //模式设定指令
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE); //设定为鼠标模式
	return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void enable_mouse(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return; /*顺利的话，键盘控制其返回ACK(0xfa)*/
}