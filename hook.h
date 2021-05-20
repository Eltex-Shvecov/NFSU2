#pragma once

#include "nitro.h"
#include <windows.h>

class NFSHook
{
public:
	NFSHook();
	~NFSHook();
	bool HookInit();
	DWORD GetBaseAdress();
	void UpdateData();
	DWORD ReadByte(std::vector<DWORD> offset);
	DWORD GetNitroValue() { return _Nitro; }
	DWORD GetEngineValue() { return _Engine; }
	DWORD GetGearValue() { return _Gear; }

private:
	DWORD _Nitro;
	DWORD _Gear;
	DWORD _Engine;
	DWORD _pid;
	HWND _winHandle;
	HANDLE _hProcess;
	DWORD _BaseAddress;
	DWORD _MajorOffset;

	TCHAR _WindowName[18];
	TCHAR _exeName[11];

	std::vector<DWORD> _nitroOffset = { 0x220, 0x45C };
	std::vector<DWORD> _engineOffset = { 0x220, 0x440 };
	std::vector<DWORD> _gearOffset = { 0x1E4 };
};
