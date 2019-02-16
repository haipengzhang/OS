; haribote-os boot asm
; TAB=4

BOTPAK	EQU		0x00280000		; bootpack‚Ìƒ[ƒhæ
DSKCAC	EQU		0x00100000		; ƒfƒBƒXƒNƒLƒƒƒbƒVƒ…‚ÌêŠ
DSKCAC0	EQU		0x00008000		; ƒfƒBƒXƒNƒLƒƒƒbƒVƒ…‚ÌêŠiƒŠƒAƒ‹ƒ‚[ƒhj

; BOOT_INFOŠÖŒW
CYLS	EQU		0x0ff0			; ƒu[ƒgƒZƒNƒ^‚ªÝ’è‚·‚é
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; F”‚ÉŠÖ‚·‚éî•ñB‰½ƒrƒbƒgƒJƒ‰[‚©H
SCRNX	EQU		0x0ff4			; ‰ð‘œ“x‚ÌX
SCRNY	EQU		0x0ff6			; ‰ð‘œ“x‚ÌY
VRAM	EQU		0x0ff8			; ƒOƒ‰ƒtƒBƒbƒNƒoƒbƒtƒ@‚ÌŠJŽn”Ô’n

		ORG		0xc200			; ‚±‚ÌƒvƒƒOƒ‰ƒ€‚ª‚Ç‚±‚É“Ç‚Ýž‚Ü‚ê‚é‚Ì‚©

; ‰æ–Êƒ‚[ƒh‚ðÝ’è

		MOV		AL,0x13			; VGAƒOƒ‰ƒtƒBƒbƒNƒXA320x200x8bitƒJƒ‰[
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; ‰æ–Êƒ‚[ƒh‚ðƒƒ‚‚·‚éiCŒ¾Œê‚ªŽQÆ‚·‚éj
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; ƒL[ƒ{[ƒh‚ÌLEDó‘Ô‚ðBIOS‚É‹³‚¦‚Ä‚à‚ç‚¤

		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

; 	PIC关闭一切中断
;	根据AT兼容机的规格，如果要初始化PIC
;	必须在CLI之前进行，否者有时会挂起
;	随后进行PIC的初始化

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; 如果连续执行OUT指令，有些机种会无法正常运行
		OUT		0xa1,AL

		CLI						; 禁止CPU级别的中断

; 为了让CPU能够访问1MB以上的内存空间，设定A20GATE
; 这段代码的作用和initkeyboard雷士，都是控制键盘控制电路，不同的指令作用不同，这里的作用是设定A20GATE

		CALL	waitkbdout		; 等同于wait_KBC_sendready
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 切换到保护模式

[INSTRSET "i486p"]				; 想要使用486指令的叙述，能使用386以后的关键字LGDT,EAX,CR0等

		LGDT	[GDTR0]			; 设定临时GDT
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; 设置bit31为0(为了禁止分页)
		OR		EAX,0x00000001	; 设置bit0为1(为了切换到保护模式)
		MOV		CR0,EAX			; 设置CR0最高位0 最低位1
		JMP		pipelineflush   ; 切换模式后流水线上的指令得重新解释一遍
pipelineflush:
		MOV		AX,1*8			;  可读写的段32bit,除了CS段寄存器都是GDT+1的段
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootpack的转送

		MOV		ESI,bootpack	; 转送源
		MOV		EDI,BOTPAK		; 转送目的地
		MOV		ECX,512*1024/4
		CALL	memcpy

; 磁盘数据最终转送到它本来的位置去

; 首先从启动扇区开始

		MOV		ESI,0x7c00		; 转送源
		MOV		EDI,DSKCAC		; 转送目的地
		MOV		ECX,512/4
		CALL	memcpy

; 所有剩下的

		MOV		ESI,DSKCAC0+512	; 转送源
		MOV		EDI,DSKCAC+512	; 转送目的地
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; 从柱面数变换为字节数/4
		SUB		ECX,512/4		; 减去IPL
		CALL	memcpy

; 必须由asmhead来完成的工作，至此全部完成
; 以后就交由bootpack来完成

; bootpack的启动

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 没有要转送的东西时
		MOV		ESI,[EBX+20]	; 转送源
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 转送目的地
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 栈初始值
		JMP		DWORD 2*8:0x0000001b

; 从0x60号设备进行IN的处理，也就是说如果控制器里面有键盘代码，或者已经积累了鼠标数据，就顺便把他们读取出来

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; AND的结果如果不是0,就跳到waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 减法运算的结果如果不是0,就跳转到memcpy
		RET

		ALIGNB	16				; ALIGNB的意思是，一直添加DB0
GDT0:
		RESB	8				; NULL Selector
		DW		0xffff,0x0000,0x9200,0x00cf	; 可以读写的段(segment)32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 可以执行的段(segment)32bit（bootpack用）

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
