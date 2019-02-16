
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void init_gdtidt(void){

	//为什么用以下两个这个地址，随机的只是这个区域没有用到
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
	int i;

	/*GDT的初始化*/
	for(i=0; i<8192; i++){
		//指针进行加法运算的时候实际上加了8，因为该结构体是8byte
		set_segmdesc(gdt+i, 0, 0, 0);
	}
	set_segmdesc(gdt+1, 0xffffffff, 0x00000000, 0x4092);//0xffffffff 4GB 全部内存
	set_segmdesc(gdt+2, 0x0007ffff, 0x00280000, 0x409a);//0x0007ffff 512kb bootpack.hrb
	load_gdtr(0xffff, 0x00270000);//借助汇编的力量给GDTR赋值

	/*IDT的初始化*/
	for (i = 0; i < 256; ++i)
	{
		set_gatedesc(idt+i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x0026f800);

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