; naskfunc
; TAB=4

[FORMAT "WCOFF"]	;制作目标文件的模式
[INSTRSET "i486p"]
[BITS 32]			;制作32位模式用的机械语言

;制作目标文件的信息

[FILE "naskfunc.nas"]		;源文件名信息
		GLOBAL _io_hlt,_write_mem8,_io_cli,_io_sti,_io_stihlt		;程序中包含的函数名
		GLOBAL _io_in8,_io_in16,_io_in32
		GLOBAL _io_out8,_io_out16,_io_out32
		GLOBAL _io_load_eflags,_io_store_eflags
		GLOBAL _load_gdtr, _load_idtr

;以下是实际的函数

[SECTION .text]		;目标文件中写了这些之后再写程序

_io_hlt:			;void io_hlt(void)
		HLT
		RET

_write_mem8			;void write_mem8(int addr);
		MOV		ECX,[ESP+4]		;[ESP+4]中存放的是地址，将其读入ECX
		MOV		AL,[ESP+8]		;[ESP+8]中存放的是数据，将其读入AL
		MOV		[ECX],AL
		RET

_io_cli:			;void io_cli(void)
		CLI
		RET

_io_sti:			;void io_sti(void)
		STI
		RET

_io_stihlt:			;void io_stihlt(void)
		STI
		HLT
		RET

_io_in8:			;void io_in8(int port);
		MOV		EDX,[ESP+4]
		MOV		EAX,0
		IN 		AL,DX
		RET

_io_in16:			;int io_in16(int port)
		MOV 	EDX,[ESP+4]
		MOV		EAX,0
		IN 		AX,DX
		RET

_io_in32:			;int io_in32(int port)
		MOV 	EDX,[ESP+4]
		MOV		EAX,0
		IN 		EAX,DX
		RET

_io_out8			;void io_out8(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		AL,[ESP+8]
		OUT 	DX,AL
		RET

_io_out16			;void io_out16(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		AL,[ESP+8]
		OUT 	DX,AX
		RET

_io_out32			;void io_out32(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		AL,[ESP+8]
		OUT 	DX,EAX
		RET

_io_load_eflags			;int io_load_eflags(void);
		PUSHFD			;指 PUSH EFLAGS
		POP		EAX
		RET

_io_store_eflags		;void io_store_eflags(int flags)
		MOV		EAX,[ESP+4]
		PUSH 	EAX
		POPFD	;指 POP EFLAGS
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET