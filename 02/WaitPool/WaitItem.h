#pragma once
#include "WaitPool.h"

enum CMDT {
	NEW,	// ��� �׸��� ���Ӱ� ���
	DEL,	// ��ϵ� ��� �׸��� ����
	EXIT	// ������ ���Ḧ ����
};

class WaitSet;
struct WAIT_ITEM {
	HANDLE		_handle;	// ó���ϰ��� �ϴ� ����ȭ ��ü �ڵ�
	PVOID		_pPrm;		// �ݹ� �Լ� �Ű������� ���޵Ǵ� ���� ���� ���� ������
	PFN_WICB	_pfnCB;		// ������ ������ �ݹ� �Լ��� ������
	WaitSet*	_base;		// �� ��� �׸��� �Ҽӵ� ��� �׸� ��Ʈ�� ������

	WAIT_ITEM(HANDLE handle, PFN_WICB pfnCB, PVOID pPrm, WaitSet* base) {
		_handle = handle;
		_pPrm = pPrm;
		_pfnCB = pfnCB;
		_base = base;
	}
};
typedef WAIT_ITEM* PWAIT_ITEM;