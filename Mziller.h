#pragma once
#include < tlhelp32.h >
#include < tchar.h >
#include < iomanip >
#include < string >
#ifndef __AFXWIN_H__
#include < Windows.h >
#include < atlstr.h >
#endif // !__AFXWIN_H__

using namespace std;

#define logbox(fmt, ...)\
CString str; \
str.Format(CString(fmt), __VA_ARGS__); \
AfxMessageBox(str);

#define endoffset -1
#define endoffset64 (DWORD64)-1

class Mziller
{
public:
	HWND GetMousePointWindow()
	{
		POINT pos;
		GetCursorPos(&pos);
		return WindowFromPoint(pos);;
	}
	void WriteMemory(void* value, HANDLE processHANDLE, ...)
	{
		DWORD valueSize = sizeof(value);
		if (value == NULL || valueSize == 0 || processHANDLE == NULL) return;
		DWORD tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD offset = 0;
		DWORD lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), 0);
		}
		va_end(addresses);

		::WriteProcessMemory(processHANDLE, (LPVOID)lastAddress, value, valueSize, NULL);
	}

	void WriteMemory(void* value, HANDLE processHANDLE, DWORD address)
	{
		WriteMemory(value, processHANDLE, address, -1);
	}

	void ReadMemory(DWORD& value, HANDLE processHANDLE, ...)
	{
		DWORD valueSize = sizeof(value);
		if (processHANDLE == NULL) return;

		DWORD tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD offset = 0;
		DWORD lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), 0);
		}

		value = tempValue;
		va_end(addresses);
	}

	void ReadMemory(DWORD& value, HANDLE processHANDLE, DWORD address)
	{
		ReadMemory(value, processHANDLE, address, -1);
	}

	void GetMemoryAddress(DWORD& address, HANDLE processHANDLE, ...)
	{
		DWORD valueSize = sizeof(address);
		if (processHANDLE == NULL) return;

		DWORD tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD offset = 0;
		DWORD lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), 0);
		}

		address = lastAddress;
		va_end(addresses);
	}

	DWORD GetModuleBase(const wchar_t* ModuleName, DWORD ProcessId)
	{
		MODULEENTRY32 ModuleEntry = { 0 };
		HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

		if (!SnapShot)
			return NULL;

		ModuleEntry.dwSize = sizeof(ModuleEntry);

		if (!Module32First(SnapShot, &ModuleEntry))
		{
			return NULL;
		}
		do
		{
			if (!wcscmp(ModuleEntry.szModule, ModuleName))
			{
				CloseHandle(SnapShot);
				return (DWORD)ModuleEntry.modBaseAddr;

			}
		}
		while (Module32Next(SnapShot, &ModuleEntry));
		CloseHandle(SnapShot);
		return NULL;
	}

	void WriteMemory64(void* value, HANDLE processHANDLE, ...)
	{
		DWORD valueSize = sizeof(valueSize);
		if (value == NULL || valueSize == 0 || processHANDLE == NULL) return;

		DWORD64 tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD64 offset = 0;
		DWORD64 lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD64)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD64), 0);
		}
		va_end(addresses);

		::WriteProcessMemory(processHANDLE, (LPVOID)lastAddress, value, valueSize, NULL);
	}

	void WriteMemory64(void* value, HANDLE processHANDLE, DWORD64 address)
	{
		WriteMemory64(value, processHANDLE, address, -1);
	}

	void ReadMemory64(DWORD64& value, HANDLE processHANDLE, ...)
	{
		DWORD64 valueSize = sizeof(value);
		if (processHANDLE == NULL) return;

		DWORD64 tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD64 offset = 0;
		DWORD64 lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD64)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD64), 0);
		}
		value = tempValue;
		va_end(addresses);
	}

	void ReadMemory64(DWORD64& value, HANDLE processHANDLE, DWORD64 address)
	{
		ReadMemory64(value, processHANDLE, address, (DWORD64)-1);
	}

	void GetMemoryAddress64(DWORD64& address, HANDLE processHANDLE, ...)
	{
		DWORD64 valueSize = sizeof(address);
		if (processHANDLE == NULL) return;

		DWORD64 tempValue = 0;
		va_list addresses;
		va_start(addresses, valueSize);
		DWORD64 offset = 0;
		DWORD64 lastAddress = 0;
		while ((offset = va_arg(addresses, DWORD64)) != -1)
		{
			lastAddress = tempValue + offset;
			::ReadProcessMemory(processHANDLE, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD64), 0);
		}

		address = lastAddress;
		va_end(addresses);
	}

	DWORD64 GetModuleBase64(const wchar_t* ModuleName, DWORD ProcessId)
	{
		MODULEENTRY32 ModuleEntry = { 0 };
		HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

		if (!SnapShot)
			return NULL;

		ModuleEntry.dwSize = sizeof(ModuleEntry);
		if (!Module32First(SnapShot, &ModuleEntry))
		{
			return NULL;
		}
		do
		{
			if (!wcscmp(ModuleEntry.szModule, ModuleName))
			{
				CloseHandle(SnapShot);
				return (DWORD64)ModuleEntry.modBaseAddr;

			}
		}
		while (Module32Next(SnapShot, &ModuleEntry));
		CloseHandle(SnapShot);
		return NULL;
	}
	DWORD Cstr_dw(CString Cstr_x)
	{
		long long_x = _ttol(Cstr_x);  //CString to char*
		DWORD DWORD_x = (DWORD)long_x; // char* to DWORD
		return DWORD_x;
	}
	CString dw_Cstr(DWORD dw_x)
	{
		CString Cstr_x;
		Cstr_x.Format(_T("%d"), dw_x);
		return Cstr_x;
	}
	float dw_float(DWORD dw_x)
	{
		float flo_x = *reinterpret_cast<float*>(&dw_x);
		return flo_x;
	}
	int Cstr_int(CString Cstr_x)
	{
		int int_x = _wtoi(Cstr_x);
		return int_x;
	}
	double Cstr_dou(CString Cstr_x)
	{
		double dou_x = _wtof(Cstr_x);
		return dou_x;
	}

	string Cstr_str(CString cstr)
	{
		string str(cstr.GetLength(), 'e');
		for (int k = 0; k < cstr.GetLength(); k++)
		{
			str[k] = (char)cstr[k];
		}
		return str;
	}
	CString str_Cstr(string str)
	{
		CString cstr;
		for (int k = 0; k < (int)str.length(); k++)
		{
			cstr += str[k];
		}
		return cstr;
	}
	string charp_str(char* charstar, int count)
	{
		string str(charstar, count);
		return str;
	}
	char* charp_str(string& str)

	{

		int str_len = str.length();

		char* charstar = new char[str_len + 1];

		for (int k = 0; k < str_len; k++)

		{

			charstar[k] = str[k];

		}

		charstar[str_len] = '\0';
		return charstar;
	}
};
