#pragma once

#include "windows.h"

struct DoWorkArgs {
	bool Cancel;
	PVOID Argument;
	PVOID Result;

	DoWorkArgs() {
		Cancel = false;
		Result = NULL;
		Argument = NULL;
	}
	DoWorkArgs(PVOID argument) {
		Cancel = false;
		Result = NULL;
		Argument = argument;
	}
};