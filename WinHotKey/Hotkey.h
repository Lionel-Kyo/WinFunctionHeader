#pragma once
#include <Windows.h>

struct _Hotkey_sturct
{
	int status;
	int key1;
	int key2;
	bool(*Getcheck)();
	void(*HotKeyfuntion)();
};

class SwitchHotkey
{
public:
	SwitchHotkey(int KEY, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY;
		key.key2 = -1;
		key.Getcheck = GETCHECK;
	}
	SwitchHotkey(int KEY1, int KEY2, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY1;
		key.key2 = KEY2;
		key.Getcheck = GETCHECK;
	}
	~SwitchHotkey()
	{
		key.status = - 1;
		CloseHandle(RHF);
		CloseHandle(CH);
	}
	void run()
	{
		RHF = CreateThread(NULL, NULL, RunHotkeyFunction, (LPVOID)&key, NULL, NULL);
		if (key.key2 == -1)
		{
			CH = CreateThread(NULL, NULL, CheckHotkey, (LPVOID)&key, NULL, NULL);
		}
		else
		{
			CH = CreateThread(NULL, NULL, CheckHotkey2, (LPVOID)&key, NULL, NULL);
		}
	}

private:
	_Hotkey_sturct key;
	HANDLE RHF, CH;

	static DWORD WINAPI RunHotkeyFunction(LPVOID structkey)
	{
		_Hotkey_sturct* key = (_Hotkey_sturct*)structkey;
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

	static DWORD WINAPI CheckHotkey(LPVOID structkey)
	{
		_Hotkey_sturct* key = (_Hotkey_sturct*)structkey;
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

	static DWORD WINAPI CheckHotkey2(LPVOID structkey)
	{
		_Hotkey_sturct* key = (_Hotkey_sturct*)structkey;
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
					if (msg.message == WM_HOTKEY && GetAsyncKeyState(key->key1))
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
};

class OnceHotkey
{
public:
	OnceHotkey(int KEY, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY;
		key.key2 = -1;
		key.Getcheck = GETCHECK;
	}
	~OnceHotkey()
	{
		CloseHandle(RHF);
	}
	void run()
	{
		RHF = CreateThread(NULL, NULL, RunHotkeyFunction, &key, NULL, NULL);
	}

private:
	_Hotkey_sturct key;
	HANDLE RHF;

	static DWORD WINAPI RunHotkeyFunction(void* structkey)
	{
		_Hotkey_sturct* key = (_Hotkey_sturct*)structkey;
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
};

class ContinuousHotkey
{
public:
	ContinuousHotkey(int KEY, void(*HOTKEYFUNCTION)(), bool(*GETCHECK)())
	{
		key.HotKeyfuntion = HOTKEYFUNCTION;
		key.status = -1;
		key.key1 = KEY;
		key.key2 = -1;
		key.Getcheck = GETCHECK;
	}
	~ContinuousHotkey()
	{
		CloseHandle(RHF);
	}
	void run()
	{
		RHF = CreateThread(NULL, NULL, RunHotkeyFunction, (LPVOID)&key, NULL, NULL);
	}

private:
	_Hotkey_sturct key;
	HANDLE RHF;

	static DWORD WINAPI RunHotkeyFunction(LPVOID structkey)
	{
		_Hotkey_sturct* key = (_Hotkey_sturct*)structkey;
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
							key->HotKeyfuntion(); break;
						default:
							break;
						}
					}
				}
			}
			if (key->Getcheck())
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