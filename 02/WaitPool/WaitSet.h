#pragma once

class WaitSet {
	friend class WaitPool;

	static DWORD WINAPI WaitSetProc(PVOID pParam);
	void InnerWaitSetProc();

	HANDLE		m_thread;	// 대기 항목의 시그걸 처리 스레드
	HANDLE		m_noti;		// 대기 항목의 추가/삭제 등의 변경을 통지하는 이벤트
	HANDLE		m_resp;		// 대기 항목 변경 통지에 대한 응답 통지 이벤트

	WAIT_ITEM*	m_arg;		// 관리 명령을 담을 임시 변수
	CMDT		m_cmd;		// 등록 또는 삭제될 WAIT_ITEM 포인터를 담을 임시 변수

	int			m_count;	// 관리 중인 동기화 객체 핸들의 수
	WaitPool*	m_base;		// WaitPool 클래스의 인스턴스의 포인터

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