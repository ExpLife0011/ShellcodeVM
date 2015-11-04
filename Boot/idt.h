#pragma once
#include  <stdint.h>

#pragma pack(push, 1)
// ��������
typedef struct GATE_DESC
{
	uint16_t	offset_low;
	uint16_t	selector;
	uint8_t		dcount;
	uint8_t		attr;
	uint16_t	offset_high;
}GATE_DESC;

struct IDTR
{
	uint16_t	limit;
	void*		base;
};

#pragma pack(pop)

#define	MAX_IDT_NUM		256

#define	IDT_TSS_GATE		0x89	//���� 386 ����״̬������ֵ
#define	IDT_CALL_GATE	0x8C	//����������ֵ
#define	IDT_INTR_GATE	0x8E	//�ж�������ֵ
#define	IDT_TRAP_GATE	0x8F	//����������ֵ			*/
//�ж��ź������ŵ�����http://blog.chinaunix.net/uid-9185047-id-445162.html
//ͨ���ж��Ž����ж�ʱ���������Զ����IF��־λ������ʱ��ջ�лָ�ԭʼ״̬
//ͨ�������Ž����ж�ʱ��IF��־λ���ֲ���


class IDT
{
private:
	GATE_DESC		m_idt[MAX_IDT_NUM];	//	256 gdt items
private:
	void set_idt_entry(int vector, int desc_type, void* handler);
public:
public:
	IDT(void);
	~IDT(void);
	void Init();
	void set_tss_gate(int vector, void* handler);
	void set_call_gate(int vector, void* handler);
	void set_intr_gate(int vector, void* handler);
	void set_trap_gate(int vector, void* handler);
};

extern IDT g_idt;