%define CODE_BASE						        0x00000000
%define REBASE(addr)                           (CODE_BASE + (addr - boot_code_start))

[bits 32]
global boot_code_start,_boot_code_start,boot_code_end,_boot_code_end

        ;ORG     0x0
        BITS    16
        SECTION .text
 boot_code_start:
_boot_code_start:
		mov		ax,0x01
		mov		bx,0x02
		mov		cx,0x03
		mov		dx,0x04
		mov		sp,0x05
		mov		bp,0x06
		mov		si,0x07
		mov		di,0x08
		hlt
;------------------------------------------------------------------
;��ʼ��ȫ�ֶ���������Ϊ���뱣��ģʽ��׼��
		 cli      ;�˴��ر��жϷǳ���Ҫ,����VMware��ִ��lidt	[IDTR]ʱ������쳣
		 lgdt	[0x1020]
		 lidt	[0x1030]

;-------------------------------------------------------------------
;��������ģʽ
         mov		eax,cr0                  
		 mov        ebx,eax
         or			eax,1
         mov		cr0,eax                        ;����PEλ
         mov        ecx,cr0

         ;���½��뱣��ģʽ... ...
         jmp       dword 0x08:REBASE(boot_code32) ;16λ��������ѡ���ӣ�32λƫ��
                                            ;����ˮ�߲����л�������
;===============================================================================
        [bits 32]               
   boot_code32:                                  
         mov		ax,		GDT32.data32                    ;�������ݶ�(4GB)ѡ����
         mov		ds,		ax
         mov		es,		ax
         mov		fs,		ax
         mov		gs,		ax
         mov		ss,		ax										;���ض�ջ��(4GB)ѡ����

		mov		eax,0x11111111
		mov		ebx,0x22222222
		mov		ecx,0x33333333
		mov		edx,0x44444444
		mov		esp,0x55555555
		mov		ebp,0x66666666
		mov		esi,0x77777777
		mov		edi,0x88888888
		hlt

align   8
GDT32:
			dq	0x0000000000000000
.code32		equ  $ - GDT32
		    dq	0x00cf9A000000ffff
.data32		equ  $ - GDT32
			dq	0x00cf92000000ffff

GDTR		dw	$ - GDT32 - 1
			dd	GDT32 ;GDT������/���Ե�ַ

IDTR		dw	0
			dd	0 ;IDT������/���Ե�ַ


 boot_code_end:
_boot_code_end:

