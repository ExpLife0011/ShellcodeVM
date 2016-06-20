#include <stdint.h>
#include <Windows.h>
#include "os.h"
#include "paging.h"

void memset(void* mem, int val, int size)
{
	for (int i = 0; i < size; i++) ((BYTE*)mem)[i] = (BYTE)val;
}

void	init_frame_database()
{//��0-16M�����ڴ���Ϊ�Ѿ�ʹ��
	memset((void*)FRAME_DB_PHYSICAL_ADDR, PAGE_FREE, FRAME_DB_SIZE);
	memset((void*)FRAME_DB_PHYSICAL_ADDR, PAGE_USED, SIZE_TO_PAGES(KERNEL_MEM_SIZE));
}

void     map_pages(uint32_t physical_address, uint32_t virtual_address, uint32_t size, uint32_t protect = PT_PRESENT | PT_WRITABLE | PT_USER)
{
	int pages = SIZE_TO_PAGES(size);
	for (int i = 0; i < pages; i++)
	{
		UINT32 pde_index = PDE_INDEX(virtual_address);
		UINT32 pte_index = PTE_INDEX(virtual_address);
		UINT32* page_table = (UINT32*)(PAGE_TABLE_PHYSICAL_ADDR + pde_index*PAGE_SIZE);
		page_table[pte_index] = physical_address | protect;
		physical_address += PAGE_SIZE;
		virtual_address += PAGE_SIZE;
	}
}

void	init_page_table()
{
	//ҳ���ڴ��ȡԤ����ģʽ��λ�������ڴ�0x00400000-0x007fffff��
	//��0x0000ҳ��ӳ��00000000-003FFFFF�����ַ
	//��0x0001ҳ��ӳ��00400000-007FFFFF�����ַ
	//��0x0300ҳ��ӳ��C0000000-C03FFFFF�����ַ(page_dir��ָ��ҳĿ¼����)
	//��0x03FFҳ��ӳ��FFC00000-FFFFFFFF�����ַ

	//����ҳ��
	memset((void*)PAGE_TABLE_PHYSICAL_ADDR, 0, PAGE_TABLE_SIZE);

	//ҳĿ¼��ӳ��
	//((UINT32*)m_page_dir_physical_addr)[PDE_INDEX(PAGE_TABLE_VIRTUAL_ADDR)] = m_page_dir_physical_addr | PT_PRESENT | PT_WRITABLE;

	//ӳ������ҳ��
	for (int i = 0; i < 1024; i++)
	{
		((UINT32*)PAGE_DIR_PHYSICAL_ADDR)[i] = (PAGE_TABLE_PHYSICAL_ADDR + i * PAGE_SIZE) | PT_PRESENT | PT_WRITABLE;
	}
	 
	map_pages(0, 0, BOOT_CODE_SIZE);
	map_pages(KERNEL_STACK_BOTTOM_PHYSICAL_ADDR, KERNEL_STACK_BOTTOM_VIRTUAL_ADDR, KERNEL_STACK_SIZE);//��ҳ֮����Ϊ�ں�ջ
	map_pages(FRAME_DB_PHYSICAL_ADDR, FRAME_DB_VIRTUAL_ADDR, FRAME_DB_SIZE);
	map_pages(VIDEO_BUF_PHYSICAL_ADDR, VIDEO_BUF_VIRTUAL_ADDR, VIDEO_BUF_SIZE);
	map_pages(PAGE_TABLE_PHYSICAL_ADDR, PAGE_TABLE_VIRTUAL_ADDR, PAGE_TABLE_SIZE);
	map_pages(OS_CODE_PHYSICAL_ADDR, OS_CODE_VIRTUAL_ADDR, OS_CODE_SIZE);
}

void   startup_paging_mode()
{
	__asm mov		eax, PAGE_DIR_PHYSICAL_ADDR
	__asm mov		cr3, eax
	__asm mov		eax, cr0
	__asm or		eax, 0x80000000
	__asm mov		cr0, eax
}

void main()
{
	init_frame_database();
	init_page_table();
	startup_paging_mode();

	VM_CONTEXT*  vm = (VM_CONTEXT*)VM_CONTEXT_VIRTUAL_ADDR;
	UINT32 OS_entry_point = vm->os_entry_point;
	__asm jmp OS_entry_point
}