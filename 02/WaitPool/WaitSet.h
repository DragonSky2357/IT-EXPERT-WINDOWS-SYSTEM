#pragma once

class WaitSet {
	friend class WaitPool;

	static DWORD WINAPI WaitSetProc(PVOID pParam);
	void InnerWaitSetProc();

	HANDLE		m_thread;	// ��� �׸��� �ñװ� ó�� ������
	HANDLE		m_noti;		// ��� �׸��� �߰�/���� ���� ������ �����ϴ� �̺�Ʈ
	HANDLE		m_resp;		// ��� �׸� ���� ������ ���� ���� ���� �̺�Ʈ

	WAIT_ITEM*	m_arg;		// ���� ����� ���� �ӽ� ����
	CMDT		m_cmd;		// ��� �Ǵ� ������ WAIT_ITEM �����͸� ���� �ӽ� ����

	int			m_count;	// ���� ���� ����ȭ ��ü �ڵ��� ��
	WaitPool*	m_base;		// WaitPool Ŭ������ �ν��Ͻ��� ������

public:
	WaitSet() {
		m_count = 0;
	}

public:
	void Init();
	void Release();
	void Command(CMDT cmd, WAIT_ITEM* arg);
};
typedef WaitSet* PWAIT_SET;