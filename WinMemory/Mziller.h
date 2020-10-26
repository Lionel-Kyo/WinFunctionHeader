#pragma once
#include <tlhelp32.h>
//#include <tchar.h>
#include <iomanip>
#include <string>
#ifndef __AFXWIN_H__
#include <Windows.h>
#include <atlstr.h> //For CString
#include <atlconv.h> //For output text
#include <atlstr.h>//For output text
#endif !// __AFXWIN_H__
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
	void outputText(CString text)
	{
		USES_CONVERSION;
		wchar_t* data = T2W(text.GetBuffer(0));
		int len = wcslen(data);

		void(*sendUnicode)(wchar_t data) = [](wchar_t data)
		{
			INPUT input[2];
			memset(input, 0, 2 * sizeof(INPUT));

			input[0].type = INPUT_KEYBOARD;
			input[0].ki.wVk = 0;
			input[0].ki.wScan = data;
			input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

			input[1].type = INPUT_KEYBOARD;
			input[1].ki.wVk = 0;
			input[1].ki.wScan = data;
			input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

			SendInput(2, input, sizeof(INPUT));
		};

		for (int i = 0; i < len; i++)
		{
			sendUnicode(data[i]);
		}
	}

	void SetClipboard(string text)
	{
		HWND hWnd = GetClipboardOwner();

		if (!OpenClipboard(hWnd))
		{
			return;
		}

		HGLOBAL pClipData;
		pClipData = GlobalAlloc(GHND, text.length() + 1);


		char* pData;
		if (pClipData == 0)return;
		pData = (char*)GlobalLock(pClipData);
		int length = text.length();
		if (pData == NULL)return;
		for (int i = 0; i < length; i++)
		{
			pData[i] = text[i];
		}

		GlobalUnlock(pClipData);

		EmptyClipboard();
		SetClipboardData(CF_TEXT, pClipData);
		CloseClipboard();
	}

	void PrintClipboard()
	{
		HWND hWnd = GetClipboardOwner();

		if (!OpenClipboard(hWnd))
		{
			return;
		}

		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HANDLE hCilpData = GetClipboardData(CF_TEXT);
			char* pData;
			pData = (char*)GlobalLock(hCilpData);
			printf("Clipboard：%s\n", pData);
			GlobalUnlock(hCilpData);
		}
		CloseClipboard();
	}

	POINT GetProcessCursor(HWND Hwnd)
	{
		POINT p;
		GetCursorPos(&p);
		::ScreenToClient(Hwnd, &p);
		return { p.x,p.y };
	}
	void SetProcessCursor(HWND Hwnd, int x, int y)
	{
		POINT p;
		p.x = x, p.y = y;
		::ClientToScreen(Hwnd, &p);
		SetCursorPos(p.x, p.y);
	}
	void SetProcessCursor_mouse(HWND Hwnd, int x, int y)
	{
		POINT p;
		p.x = x, p.y = y;
		::ClientToScreen(Hwnd, &p);
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx(p.x, desktop), dy(p.y, desktop), 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx(p.x, desktop), dy(p.y, desktop), 0, 0);
	}
	void press_key(DWORD pKey)
	{
		keybd_event((BYTE)pKey, scan_code(pKey), 0, 0);
	}
	void release_key(DWORD pKey)
	{
		keybd_event((BYTE)pKey, scan_code(pKey), KEYEVENTF_KEYUP, 0);
	}
	void MouseLeftClick(int times, int updowndelay,int delay)
	{
		for (int k = 0; k < times; k++)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(updowndelay);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(delay);
		}
	}
	void MouseRightClick(int times, int updowndelay, int delay)
	{
		for (int k = 0; k < times; k++)
		{
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			Sleep(updowndelay);
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			Sleep(delay);
		}
	}
	void MouseMiddleClick(int times, int updowndelay, int delay)
	{
		for (int k = 0; k < times; k++)
		{
			mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
			Sleep(updowndelay);
			mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
			Sleep(delay);
		}
	}
	void MouseLeftClick()
	{
		MouseLeftClick(1, 0, 0);
	}
	void MouseRightClick()
	{
		MouseRightClick(1, 0, 0);
	}
	void MouseMiddleClick()
	{
		MouseMiddleClick(1, 0, 0);
	}
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

		if (lastAddress == 0)return;
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

		if (lastAddress == 0)return;
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
	float dw_float(DWORD dw_x)
	{
		float flo_x = *reinterpret_cast<float*>(&dw_x);
		return flo_x;
	}

	string charp_str(char* charstar, int count)
	{
		string str(charstar, count);
		return str;
	}
	char* str_charp(string str)
	{
		int str_len = str.length();
		char* charp = new char[str_len + 1];
		for (int k = 0; k < str_len; k++)
		{
			charp[k] = str[k];
		}
		charp[str_len] = '\0';
		return charp;
	}
	DWORD Cstr_dw(CString Cstr_x)
	{
		long long_x = _ttol(Cstr_x);  //CString to long
		DWORD DWORD_x = (DWORD)long_x; // long to DWORD
		return DWORD_x;
	}
	CString dw_CStr(DWORD dw_x)
	{
		CString Cstr_x;
		Cstr_x.Format(_T("%d"), dw_x);
		return Cstr_x;
	}
	int CStr_int(CString Cstr_x)
	{
		int int_x = _wtoi(Cstr_x);
		return int_x;
	}
	double CStr_dou(CString Cstr_x)
	{
		double dou_x = _wtof(Cstr_x);
		return dou_x;
	}

	string CStr_str(CString cstr)
	{
		string str(cstr.GetLength(), 0);
		for (int k = 0; k < cstr.GetLength(); k++)
		{
			str[k] = (char)cstr[k];
		}
		return str;
	}
	CString str_CStr(string str)
	{
		CString cstr;
		int len = str.length();
		for (int k = 0; k < len; k++)
		{
			cstr += str[k];
		}
		return cstr;
	}
	char* CStr_charp(CString cstr)
	{
		int len = cstr.GetLength();
		char* charp = new char[len + 1];
		for (int k = 0; k < len; k++)
		{
			charp[k] = (char)cstr[k];
		}
		charp[len] = '\0';
		return charp;
	}
	string toString(CString cstr)
	{
#ifdef _UNICODE
		USES_CONVERSION;
		std::string str(W2A(cstr));
		return str;
#else
		std::string str(cstr.GetBuffer());
		cstr.ReleaseBuffer();
		return str;
#endif // _UNICODE 
	}

	CString toCString(string str)
	{
#ifdef _UNICODE
		USES_CONVERSION;
		CString cstr(str.c_str());
		return cstr;
#else
		CString cstr;
		cstr.Format("%s", str.c_str());
		return cstr;
#endif // _UNICODE  
	}

private:
	BYTE scan_code(DWORD pKey)
	{
		const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
		return (BYTE)result;
	}
	int dx(int p_x, RECT desktop)
	{
		double x = p_x * (65536.0 / desktop.right);
		return int(x);
	}

	int dy(int p_y, RECT desktop)
	{
		double y = p_y * (65536.0 / desktop.bottom);
		return int(y);
	}
};
