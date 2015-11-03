
        ORG     0x7C00
		BITS    16
        SECTION .text
 start:
		jmp		boot_code16 ;5 byte
		nop
guest_loader_main dd 0
boot_code16:
;------------------------------------------------------------------
;��ʼ��ȫ�ֶ���������Ϊ���뱣��ģʽ��׼��
		 cli      ;�˴��ر��жϷǳ���Ҫ,����VMware��ִ��lidt	[IDTR]ʱ������쳣
		 lgdt	[GDTR]
		 lidt	[IDTR]
;-------------------------------------------------------------------
;��������ģʽ
         mov		eax,cr0                  
         or			eax,1
         mov		cr0,eax                        ;����PEλ

         ;���½��뱣��ģʽ... ...
         jmp       dword GDT32.code32:boot_code32 ;16λ��������ѡ���ӣ�32λƫ��
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
		 mov        esp,    0x10000
		 mov        eax,    guest_loader_main
		 call       eax

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


