
#include <stdio.h>
#include "bootpack.h"

void HariMain(void)
{
	struct  BOOTINFO *binfo = (struct BOOTINFO *)0xff0;
	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();
	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	sprintf(s, "scrnx = %d", binfo->scrnx);
	putfont8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

	for (;;) {
		io_hlt();
	}

}

