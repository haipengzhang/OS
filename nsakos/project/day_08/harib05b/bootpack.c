
#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo, mousefifo;

struct MOUSE_DEC
{
	unsigned char buf[3], phase;
};
void enable_mouse(struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);
void init_keyboard(void);

void HariMain(void)
{
	struct  BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];;
	int mx, my, i;
	struct MOUSE_DEC mdec;

	init_gdtidt();
	init_pic();//设置pic的寄存器
	io_sti();//设置cpu接收外部中断
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
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
	
	enable_mouse(&mdec);

	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484,  0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo) != 0) {
				i = fifo8_get(&mousefifo);
				io_sti();
				/*等待鼠标的0xfa的状态*/
				if(mouse_decode(&mdec, i) != 0){
					/*3字节都凑齐了，所以把它们显示出来*/
					/*鼠标的3个字节都齐了，显示出来*/
					sprintf(s, "%02X %02X %02X", mdec.buf[0], mdec.buf[1], mdec.buf[2]);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 8 * 8 - 1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
				}
			}
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

void enable_mouse(struct MOUSE_DEC *mdec)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/*顺利的话，键盘控制其返回ACK(0xfa)*/
	mdec->phase = 0; /* 等待0xfa的阶段 */
	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat){
	if (mdec->phase == 0)
	{
		/*等待鼠标的0xfa的阶段*/
		if (dat == 0xfa)
		{
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1)
	{
		/* 等待鼠标第一字节的阶段 */
		mdec->buf[0] = dat;
		mdec->phase = 2;
		return 0;
	}
	if(mdec->phase == 2){
		/* 等待鼠标第一字节的阶段 */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if(mdec->phase == 3){
		/* 等待鼠标第一字节的阶段 */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		return 1;
	}
	return -1; /*应该不可能到这里来*/
}
