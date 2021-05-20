#define _CRT_SECURE_NO_WARNINGS

#include "hook.h"
#include "nitro.h"

#include <iostream>
#include <tchar.h>
#include <psapi.h>


NFSHook::NFSHook()
{
	_Nitro = 0;
	_Gear = 0;
	_Engine = 0;
	_pid = NULL;
	_winHandle = NULL;
	_hProcess = NULL;
	_BaseAddress = 0x0;
	_MajorOffset = 0x49CEB0;

	_tcscpy(_WindowName, _T("NFS Underground 2"));
	_tcscpy(_exeName, _T("SPEED2.EXE"));
}

NFSHook::~NFSHook()
{
	CloseHandle(_hProcess);
}

bool NFSHook::HookInit()
{
	_winHandle = FindWindow(NULL, _WindowName);
	if (!_winHandle)
	{
		std::cout << "FindWindow error" << std::endl;
		return false;
	}

	GetWindowThreadProcessId(_winHandle, &_pid);
	if (!_pid)
	{
		std::cout << "GetWindowThreadProcessId error" << std::endl;
		return false;
	}

	_hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, _pid);
	if (!_hProcess)
	{
		std::cout << "OpenProcess error" << std::endl;
		return false;
	}

	_BaseAddress = GetBaseAdress();
	if (_BaseAddress == 0x0)
	{
		std::cout << "GetBaseAddress error" << std::endl;
		return false;
	}

	return true;
}

DWORD NFSHook::GetBaseAdress()
{
	DWORD Base = 0x0;
	HMODULE hMods[1024];

	if (EnumProcessModules(_hProcess, hMods, sizeof(hMods), &_pid) == 0)
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < (_pid / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(_hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				if (_tcsstr(szModName, _exeName))
				{
					Base = (DWORD)hMods[i];
				}
			}
		}
	}

	return Base;
}

void NFSHook::UpdateData()
{
	_Nitro = ReadByte(_nitroOffset);
	auto temp_engine = ReadByte(_engineOffset);
	_Engine = (int)*((float *)&temp_engine);
	_Gear = ReadByte(_gearOffset);
}

DWORD NFSHook::ReadByte(std::vector<DWORD> offset)
{
	DWORD value = 0;

	if (ReadProcessMemory(_hProcess, (LPVOID)(_BaseAddress + _MajorOffset), &value, sizeof(DWORD), NULL))
	{
		for (int i = 0; i < offset.size(); i++)
		{
			ReadProcessMemory(_hProcess, (LPVOID)(value + offset[i]), &value, sizeof(DWORD), NULL);
		}
		return value;
	}
	else
	{
		return 8000;
	}
}
