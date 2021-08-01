#pragma once
#include "WaitPool.h"

enum CMDT {
	NEW,	// 대기 항목을 새롭게 등록
	DEL,	// 등록된 대기 항목을 삭제
	EXIT	// 스레드 종료를 통지
};

class WaitSet;
struct WAIT_ITEM {
	HANDLE		_handle;	// 처리하고자 하는 동기화 객체 핸들
	PVOID		_pPrm;		// 콜백 함수 매개변수로 전달되는 유저 정의 참조 포인터
	PFN_WICB	_pfnCB;		// 유저가 정의할 콜백 함수의 포인터
	WaitSet*	_base;		// 본 대기 항목이 소속된 대기 항목 세트의 포인터

	WAIT_ITEM(HANDLE handle, PFN_WICB pfnCB, PVOID pPrm, WaitSet* base) {
		_handle = handle;
		_pPrm = pPrm;
		_pfnCB = pfnCB;
		_base = base;
	}
};
typedef WAIT_ITEM* PWAIT_ITEM;