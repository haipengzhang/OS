
#include "bootpack.h"

void init_gdtidt(void){

	//为什么用以下两个这个地址，随机的只是这个区域没有用到
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) ADR_IDT;
	int i;

	/*GDT的初始化*/
	for(i=0; i<LIMIT_GDT/8; i++){
		//指针进行加法运算的时候实际上加了8，因为该结构体是8byte
		set_segmdesc(gdt+i, 0, 0, 0);
	}
	set_segmdesc(gdt+1, 0xffffffff, 0x00000000, AR_DATA32_RW);//0xffffffff 4GB 全部内存
	set_segmdesc(gdt+2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);//0x0007ffff 512kb bootpack.hrb
	load_gdtr(LIMIT_GDT, ADR_GDT);//借助汇编的力量给GDTR赋值

	/*IDT的初始化*/
	for (i = 0; i < LIMIT_IDT; ++i)
	{
		set_gatedesc(idt+i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);

	return;

}
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar){

	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;

}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar){

	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
	
}