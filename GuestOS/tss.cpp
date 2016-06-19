#include "tss.h"
#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include "os.h"

TSS::TSS()
{
	//�ڳ����������TSS�����ñ�Ҫ����Ŀ 
	memset(this, 0, sizeof(TSS)); 
}

TSS::~TSS()
{
}

void TSS::Init(GDT * gdt)
{
	//�ڳ����������TSS�����ñ�Ҫ����Ŀ 
	memset(this, 0, sizeof(TSS)); 

	m_back_link = 0;//������=0
	m_esp0 = KERNEL_STACK_TOP_VIRTUAL_ADDR;
	m_ss0 = SEL_KERNEL_DATA;
	m_cr3 = __readcr3();//�Ǽ�CR3(PDBR)
	m_eip = 0;//(uint32)UserProcess;
	m_eflags = 0x00000202;

	m_ldt = 0;//û��LDT������������û��LDT������
	m_trap = 0;
	m_iobase = 104;//û��I/Oλͼ��0��Ȩ����ʵ�ϲ���Ҫ��
				   //���������������TSS������������װ��GDT�� 
	m_tss_seg = gdt->add_tss_entry((uint32)this, 103);
}

void TSS::active()
{
	//����Ĵ���TR�е�������������ڵı�־��������Ҳ�����˵�ǰ������˭��
	//�����ָ��Ϊ��ǰ����ִ�е�0��Ȩ�����񡰳������������������TSS����
	__asm	mov		ecx,	this
	__asm	mov		ax,		[ecx]TSS.m_tss_seg //index = 5
	__asm	ltr		ax
	//printf("TSS Register() OK\n");
}