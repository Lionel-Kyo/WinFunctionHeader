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
	void press_key(const char* key)
	{
		press_key(keycode(key));
	}
	void release_key(const char* key)
	{
		release_key(keycode(key));
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
private:
	BYTE scan_code(DWORD pKey)
	{
		const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
		return (BYTE)result;
	}
	DWORD keycode(const char* key)
	{
		int pkey = 0;
		int temp = 0;
		if (key[1] == '\0')
		{
			temp = (int)key[0];
			if (temp >= 97 && temp <= 122)pkey = temp - 32;
			if (temp >= 48 && temp <= 57)pkey = temp;
			if (temp >= 42 && temp <= 47 && temp != 44)pkey = temp + 64;
		}
		else if (key[4] == '\0' && key[0] == 'n' && key[1] == 'u' && key[2] == 'm')
		{
			temp = (int)key[3];
			if (temp >= 48 && temp <= 57)pkey = temp + 48;
		}
		else if (key[2] == '\0' && (key[0] == 'F' || key[0] == 'f'))
		{
			temp = (int)key[1];
			if (temp >= 49 && temp <= 57)pkey = temp + 63;
		}
		else if (key[3] == '\0' && (key[0] == 'F' || key[0] == 'f') && key[1] == '1')
		{
			temp = (int)key[2];
			if (temp >= 48 && temp <= 50)pkey = temp + 73;
		}
		else if (key == "numenter" || key == "NumEnter")pkey = 108;
		else if (key == "backspace" || key == "Backspace")pkey = 8;
		else if (key == "tab" || key == "Tab")pkey = 9;
		else if (key == "enter" || key == "Enter")pkey = 13;
		else if (key == "shift" || key == "Shift")pkey = 16;
		else if (key == "ctrl" || key == "control" || key == "Control" || key == "Ctrl") pkey = 17;
		else if (key == "alt" || key == "Alt") pkey = 18;
		else if (key == "caps" || key == "Caps") pkey = 20;
		else if (key == "Esc" || key == "esc") pkey = 27;
		else if (key == "space" || key == "Space") pkey = 32;
		else if (key == "pgup" || key == "pageup" || key == "Pgup" || key == "PageUp") pkey = 33;
		else if (key == "pgdn" || key == "pagedown" || key == "Pgdn" || key == "PageDown") pkey = 34;
		else if (key == "end" || key == "End") pkey = 35;
		else if (key == "Home" || key == "home") pkey = 36;
		else if (key == "left" || key == "Left") pkey = 37;
		else if (key == "up" || key == "Up") pkey = 38;
		else if (key == "right" || key == "Right") pkey = 39;
		else if (key == "down" || key == "Down" || key == "dn") pkey = 40;
		else if (key == "insert" || key == "Insert") pkey = 45;
		else if (key == "delete" || key == "Delete" || key == "Del" || key == "del") pkey = 46;
		else if (key == "numlock" || key == "Numlock" || key == "NumLock" || key == "numLock") pkey = 144;
		else
		{
			return -1;
		}
		return (DWORD)pkey;
	}
	int dx(int p_x, RECT desktop)
	{
		float x = p_x * (65536.0 / desktop.right);
		return int(x);
	}

	int dy(int p_y, RECT desktop)
	{
		float y = p_y * (65536.0 / desktop.bottom);
		return int(y);
	}
};
