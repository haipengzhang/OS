
#include <stdio.h>
#include "bootpack.h"

void HariMain(void)
{
	struct  BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();//设置gdt表和idt表，其中还设置了gdtr和idtr, 而且设定了3个中断处理函数
	init_pic();//设置pic的寄存器
	io_sti();//设置接收外部中断(cpu级别)

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	// sprintf(s, "scrnx = %d", binfo->scrnx);
	// putfont8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	
	// 修改了pic的imr寄存器，设置接收外部中断(PIC级别)
	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	for (;;) {
		io_hlt();
	}

}

