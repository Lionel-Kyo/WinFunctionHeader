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
	void outputText(string text)
	{
		for (int n = 0; n < text.length(); n++)
		{
			BYTE scan_code, scan_shift;
			DWORD vk = text[n];
			bool shift = false;
			if (vk >= 97 && vk <= 122) vk -= 32;
			else if (vk >= 65 && vk <= 90)shift = true;
			else if (vk == 45)vk = VK_OEM_MINUS;
			else if (vk == 61)vk = VK_OEM_PLUS;
			else if (vk == 91)vk = VK_OEM_4;
			else if (vk == 93)vk = VK_OEM_6;
			else if (vk == 92)vk = VK_OEM_5;
			else if (vk == 59)vk = VK_OEM_1;
			else if (vk == 39)vk = VK_OEM_7;
			else if (vk == 44)vk = VK_OEM_COMMA;
			else if (vk == 46)vk = VK_OEM_PERIOD;
			else if (vk == 47)vk = VK_OEM_2;
			else if (vk == 96)vk = VK_OEM_3;
			else if (vk == 33)
			{
				vk = '1';
				shift = true;
			}
			else if (vk == 64)
			{
				vk = '2';
				shift = true;
			}
			else if (vk == 35)
			{
				vk = '3';
				shift = true;
			}
			else if (vk == 36)
			{
				vk = '4';
				shift = true;
			}
			else if (vk == 37)
			{
				vk = '5';
				shift = true;
			}
			else if (vk == 94)
			{
				vk = '6';
				shift = true;
			}
			else if (vk == 38)
			{
				vk = '7';
				shift = true;
			}
			else if (vk == 42)
			{
				vk = '8';
				shift = true;
			}
			else if (vk == 40)
			{
				vk = '9';
				shift = true;
			}
			else if (vk == 41)
			{
				vk = '0';
				shift = true;
			}
			else if (vk == 95)
			{
				vk = VK_OEM_MINUS;
				shift = true;
			}
			else if (vk == 43)
			{
				vk = VK_OEM_PLUS;
				shift = true;
			}
			else if (vk == 123)
			{
				vk = VK_OEM_4;
				shift = true;
			}
			else if (vk == 125)
			{
				vk = VK_OEM_6;
				shift = true;
			}
			else if (vk == 124)
			{
				vk = VK_OEM_5;
				shift = true;
			}
			else if (vk == 58)
			{
				vk = VK_OEM_1;
				shift = true;
			}
			else if (vk == 34)
			{
				vk = VK_OEM_7;
				shift = true;
			}
			else if (vk == 60)
			{
				vk = VK_OEM_COMMA;
				shift = true;
			}
			else if (vk == 62)
			{
				vk = VK_OEM_PERIOD;
				shift = true;
			}
			else if (vk == 63)
			{
				vk = VK_OEM_2;
				shift = true;
			}
			else if (vk == 126)
			{
				vk = VK_OEM_3;
				shift = true;
			}
			if (shift)
			{
				scan_shift = (BYTE)MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
				keybd_event((BYTE)VK_LSHIFT, scan_shift, 0, 0);
			}
			scan_code = (BYTE)MapVirtualKey(vk, MAPVK_VK_TO_VSC);
			keybd_event((BYTE)vk, scan_code, 0, 0);
			keybd_event((BYTE)vk, scan_code, KEYEVENTF_KEYUP, 0);
			if (shift)keybd_event((BYTE)VK_LSHIFT, scan_shift, KEYEVENTF_KEYUP, 0);
		}
	}
	void SetClipboard(std::string text)
	{
		HWND hWnd = GetClipboardOwner();

		if (!OpenClipboard(hWnd))
		{
			return;
		}

		HGLOBAL pClipData;
		pClipData = GlobalAlloc(GHND, text.length() + 1);


		char* pData;
		pData = (char*)GlobalLock(pClipData);
		for (int i = 0; i < text.length(); i++)
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
			printf("Clipboard：%s", pData);
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
		long long_x = _ttol(Cstr_x);  //CString to long
		DWORD DWORD_x = (DWORD)long_x; // long to DWORD
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
		string str(cstr.GetLength(), 'a');
		for (int k = 0; k < cstr.GetLength(); k++)
		{
			str[k] = (char)cstr[k];
		}
		return str;
	}
	CString str_Cstr(string str)
	{
		CString cstr;
		//for (int k = 0; k < (int)str.length(); k++)
		//{
		//	cstr += str[k];
		//}
		cstr.Format(L"%s", str);
		return cstr;
	}
	string charp_str(char* charstar, int count)
	{
		string str(charstar, count);
		return str;
	}
	char* str_charp(string str)
	{
		//int str_len = str.length();
		//char* charstar = new char[str_len + 1];
		//for (int k = 0; k < str_len; k++)
		//{
		//	charstar[k] = str[k];
		//}
		//charstar[str_len] = '\0';
		char* charp = (char*)str.c_str();
		return charp;
	}
	char* Cstr_charp(CString cstr)
	{
		string str = Cstr_str(cstr);
		char* charp = (char*)str.c_str();
		return charp;
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
