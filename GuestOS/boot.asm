%define  OS_LOADER_BASE			0x00400000
%define  BOOT_STACK_BASE		0x00010000
%define  REBASE16(addr)        ($$ + (addr - boot_code16))
%define  REBASE32(addr)        ($$ + (addr - boot_code16))

[BITS 32]
global boot_code16
extern  _main

[BITS 16]
        SECTION .text

boot_code16:
 osloader_start:
_osloader_start:
;------------------------------------------------------------------
;��ʼ��ȫ�ֶ���������Ϊ���뱣��ģʽ��׼��
		 cli      ;�˴��ر��жϷǳ���Ҫ,����VMware��ִ��lidt	[IDTR]ʱ������쳣
		 a32 lgdt	[cs:REBASE16(GDTR)]
		 a32 lidt	[cs:REBASE16(IDTR)]
;-------------------------------------------------------------------
;��������ģʽ
         mov		eax,cr0                  
         or			eax,1
         mov		cr0,eax                        ;����PEλ

         ;���½��뱣��ģʽ... ...
         jmp       dword GDT32.code32:REBASE32(boot_code32);16λ��������ѡ���ӣ�32λƫ��
                                                    ;����ˮ�߲����л�������
;===============================================================================
[BITS 32]               
boot_code32:                                  
         mov		ax,		GDT32.data32                    ;�������ݶ�(4GB)ѡ����
         mov		ds,		ax
         mov		es,		ax
         mov		fs,		ax
         mov		gs,		ax
         mov		ss,		ax										;���ض�ջ��(4GB)ѡ����

		 ;����ջָ��
		 mov        esp,    BOOT_STACK_BASE
		 call		_main
		 jmp		$

		 ;ͨ��PEͷ����ȡ��ں�����ַ
		 mov        esi,	OS_LOADER_BASE;esi = PIMAGE_DOS_HEADER  
		 mov		eax,    [esi + 0x3C] ; eax = PIMAGE_NT_HEADERS
		 mov        eax,    [esi + eax + 0x18 + 0x10];eax =  PIMAGE_NT_HEADERS->IMAGE_OPTIONAL_HEADER.AddressOfEntryPoint
		 add        eax,	esi 
		 call       eax
		 jmp        $

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


