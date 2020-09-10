#pragma once
#include <Windows.h>


#define SwitchHotkeySingle 1
#define SwitchHotkeyDual 2
#define ContinuousHotkey 3
#define OnceHotkey 4

struct _Hotkey_struct
{
	int status;
	int key1;
	int key2;
	bool(*Getcheck)();
	void(*HotKeyfuntion)();
};

class SetHotkey
{
public:
	SetHotkey(int KEY1, int KEY2, int MODE, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY1;
		key.key2 = KEY2;
		key.Getcheck = GETCHECK;
		RHF = 0;
		SHC = 0;
		mode = MODE;
	}
	SetHotkey(int KEY1, int MODE, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY1;
		key.key2 = -1;
		key.Getcheck = GETCHECK;
		RHF = 0;
		SHC = 0;
		mode = MODE;
	}
	~SetHotkey()
	{
		key.status = -1;
		BYTE result = (BYTE)MapVirtualKey(key.key1, MAPVK_VK_TO_VSC);
		keybd_event((BYTE)(key.key1), result, 0, 0);
		keybd_event((BYTE)(key.key1), result, KEYEVENTF_KEYUP, 0);
		CloseHandle(RHF);
		CloseHandle(SHC);
	}
	void run()
	{
		if (mode == 1 || mode == 2)
		{
			SHC = CreateThread(NULL, NULL, SwitchHotkeyCheck, (LPVOID)&key, NULL, NULL);
			if (key.key2 == -1)
			{
				RHF = CreateThread(NULL, NULL, RunSwitchHotkey, (LPVOID)&key, NULL, NULL);
			}
			else
			{
				RHF = CreateThread(NULL, NULL, RunSwitchHotkey2, (LPVOID)&key, NULL, NULL);
			}
		}
		else if (mode == 3)
		{
			RHF = CreateThread(NULL, NULL, RunContinuousHotkey, (LPVOID)&key, NULL, NULL);
		}
		else if (mode == 4)
		{
			RHF = CreateThread(NULL, NULL, RunOnceHotkey, (LPVOID)&key, NULL, NULL);
		}
	}

private:
	int mode;
	_Hotkey_struct key;
	HANDLE RHF, SHC;

	static DWORD WINAPI SwitchHotkeyCheck(LPVOID structkey)
	{
		_Hotkey_struct* key = (_Hotkey_struct*)structkey;
		while (true)
		{
			if (key->status == 1)
			{
				key->HotKeyfuntion();
			}
			if (!key->Getcheck())return NULL;
			if (key->status == -1)Sleep(50);
		}
		return NULL;
	}

	static DWORD WINAPI RunSwitchHotkey(LPVOID structkey)
	{
		_Hotkey_struct* key = (_Hotkey_struct*)structkey;
		MSG msg = { 0 };
		HWND hConsole = GetActiveWindow();
		while (true)
		{
			if (key->Getcheck())
			{
				RegisterHotKey(hConsole, 1, MOD_NOREPEAT, key->key1);
				RegisterHotKey(hConsole, 2, MOD_NOREPEAT | MOD_CONTROL, key->key1);
				while (GetMessage(&msg, NULL, 0, 0) != 0)
				{
					if (!key->Getcheck())break;
					if (msg.message == WM_HOTKEY && GetAsyncKeyState(key->key1))
					{
						switch (msg.wParam)
						{
						case 1:
						case 2:
							key->status *= -1; break;
						default:
							break;
						}
					}
					while (GetAsyncKeyState(key->key1))Sleep(25);
				}
			}
			if (!key->Getcheck())
			{
				UnregisterHotKey(hConsole, 1);
				UnregisterHotKey(hConsole, 2);
				key->status = -1;
				break;
			}
			Sleep(250);
		}
		return NULL;
	}

	static DWORD WINAPI RunSwitchHotkey2(LPVOID structkey)
	{
		_Hotkey_struct* key = (_Hotkey_struct*)structkey;
		MSG msg = { 0 };
		HWND hConsole = GetActiveWindow();
		while (true)
		{
			if (key->Getcheck())
			{
				RegisterHotKey(hConsole, 1, MOD_NOREPEAT, key->key1);
				RegisterHotKey(hConsole, 2, MOD_NOREPEAT | MOD_CONTROL, key->key1);
				RegisterHotKey(hConsole, 3, MOD_NOREPEAT, key->key2);
				RegisterHotKey(hConsole, 4, MOD_NOREPEAT | MOD_CONTROL, key->key2);
				while (GetMessage(&msg, NULL, 0, 0) != 0)
				{
					if (!key->Getcheck())break;
					if (msg.message == WM_HOTKEY && (GetAsyncKeyState(key->key1) || GetAsyncKeyState(key->key2)))
					{
						switch (msg.wParam)
						{
						case 1:
						case 2:
							key->status = 1; break;
						case 3:
						case 4:
							key->status = -1; break;
						default:
							break;
						}
					}
					while (GetAsyncKeyState(key->key1))Sleep(25);
				}
			}
			if (!key->Getcheck())
			{
				UnregisterHotKey(hConsole, 1);
				UnregisterHotKey(hConsole, 2);
				UnregisterHotKey(hConsole, 3);
				UnregisterHotKey(hConsole, 4);
				key->status = -1;
				break;
			}
			Sleep(250);
		}
		return NULL;
	}

	static DWORD WINAPI RunOnceHotkey(void* structkey)
	{
		_Hotkey_struct* key = (_Hotkey_struct*)structkey;
		MSG msg = { 0 };
		HWND hConsole = GetActiveWindow();
		while (true)
		{
			if (key->Getcheck())
			{
				RegisterHotKey(hConsole, 1, MOD_NOREPEAT, key->key1);
				RegisterHotKey(hConsole, 2, MOD_NOREPEAT | MOD_CONTROL, key->key1);
				while (GetMessage(&msg, NULL, 0, 0) != 0)
				{
					if (!key->Getcheck())break;
					if (msg.message == WM_HOTKEY && GetAsyncKeyState(key->key1))
					{
						switch (msg.wParam)
						{
						case 1:
						case 2:
							key->HotKeyfuntion(); break;
						default:
							break;
						}
					}
					while (GetAsyncKeyState(key->key1))Sleep(25);
				}
			}
			if (!key->Getcheck())
			{
				UnregisterHotKey(hConsole, 1);
				UnregisterHotKey(hConsole, 2);
				break;
			}
			Sleep(250);
		}
		return NULL;
	}

	static DWORD WINAPI RunContinuousHotkey(LPVOID structkey)
	{
		_Hotkey_struct* key = (_Hotkey_struct*)structkey;
		MSG msg = { 0 };
		HWND hConsole = GetActiveWindow();
		while (true)
		{
			if (key->Getcheck())
			{
				RegisterHotKey(hConsole, 1, MOD_NOREPEAT, key->key1);
				RegisterHotKey(hConsole, 2, MOD_NOREPEAT | MOD_CONTROL, key->key1);
				while (GetMessage(&msg, NULL, 0, 0) != 0)
				{
					if (!key->Getcheck())break;
					if (msg.message == WM_HOTKEY)
					{
						switch (msg.wParam)
						{
						case 1:
						case 2:
							while (GetAsyncKeyState(key->key1)) { key->HotKeyfuntion(); }
							break;
						default:
							break;
						}
					}
				}
			}
			if (!key->Getcheck())
			{
				UnregisterHotKey(hConsole, 1);
				UnregisterHotKey(hConsole, 2);
				break;
			}
			Sleep(250);
		}
		return NULL;
	}
};