#pragma once
#include <Windows.h>
#include <thread>

#define SwitchHotkeySingle 1
#define SwitchHotkeyDual 2
#define ContinuousHotkey 3
#define OnceHotkey 4

class SetHotkey
{
public:
	SetHotkey()
	{
		SET(0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	~SetHotkey()
	{
		*Check = false;
		UnregisterHotKey(hConsole, 1);
		if (key2 != -1)
		{
			UnregisterHotKey(hConsole, 2);
		}
		hConsole = 0;
		status = -1;
		key1 = 0;
		key2 = 0;
		mode = 0;
		block = false;
		ctrl = false;
		shift = false;
		alt = false;
		HotKeyfuntion = 0;
	}
	void set(int KEY1, int KEY2, int MODE, bool BlockOrignalKey, int CTRL, int SHIFT, int ALT, void(*HOTKEYFUNCTION)(), bool* check)
	{
		SET(KEY1, KEY2, MODE, BlockOrignalKey, CTRL, SHIFT, ALT, HOTKEYFUNCTION, check);
	}
	void set(int KEY1, int KEY2, int MODE, void(*HOTKEYFUNCTION)(), bool* check)
	{
		SET(KEY1, KEY2, MODE, 1, 0, 0, 0, HOTKEYFUNCTION, check);
	}

	void set(int KEY1, int MODE, bool BlockOrignalKey, int CTRL, int SHIFT, int ALT, void(*HOTKEYFUNCTION)(), bool* check)
	{
		SET(KEY1, -1, MODE, BlockOrignalKey, CTRL, SHIFT, ALT, HOTKEYFUNCTION, check);
	}
	void set(int KEY1, int MODE, void(*HOTKEYFUNCTION)(), bool* check)
	{
		SET(KEY1, -1, MODE, 1, 0, 0, 0, HOTKEYFUNCTION, check);
	}

	void run()
	{
		if (block)
		{
			//hConsole = ::GetActiveWindow();
			RegisterHotKey(hConsole, 1, MOD_NOREPEAT, key1);
			if (key2 != -1)
			{
				RegisterHotKey(hConsole, 2, MOD_NOREPEAT, key2);
			}
		}
		if (mode == 1 || mode == 2)
		{
			std::thread RSH(&SetHotkey::RunSwitchHotkey, this);
			RSH.detach();
			if (key2 == -1)
			{
				std::thread CSH(&SetHotkey::CheckSwitchHotkey1, this);
				CSH.detach();
			}
			else
			{
				std::thread CSH(&SetHotkey::CheckSwitchHotkey2, this);
				CSH.detach();
			}
		}
		else if (mode == 3)
		{
			std::thread RCH(&SetHotkey::RunContinuousHotkey, this);
			RCH.detach();
		}
		else if (mode == 4)
		{
			std::thread ROH(&SetHotkey::RunOnceHotkey, this);
			ROH.detach();
		}
	}

private:
	HWND hConsole;
	bool block;
	int ctrl;
	int shift;
	int alt;
	int status;
	int key1;
	int key2;
	int mode;
	void(*HotKeyfuntion)();
	bool* Check;

private:
	void SET(int KEY1, int KEY2, int MODE, bool BlockOrignalKey, int CTRL, int SHIFT, int ALT, void(*HOTKEYFUNCTION)(), bool* check)
	{
		hConsole = 0;
		status = -1;
		key1 = KEY1;
		key2 = KEY2;
		mode = MODE;
		block = BlockOrignalKey;
		ctrl = CTRL;
		shift = SHIFT;
		alt = ALT;
		HotKeyfuntion = HOTKEYFUNCTION;
		Check = check;
	}
	bool GetKeyStatus(int vk_code)
	{
		bool check = GetAsyncKeyState(vk_code);
		if (ctrl == 1) { check = check && GetAsyncKeyState(VK_CONTROL); }
		else if(ctrl == 0) { check = check && !(GetAsyncKeyState(VK_CONTROL)); }
		if (shift == 1) { check = check && GetAsyncKeyState(VK_SHIFT); }
		else if (shift == 0) { check = check && !(GetAsyncKeyState(VK_SHIFT)); }
		if (alt == 1) { check = check && GetAsyncKeyState(VK_MENU); }
		else if (alt == 0) { check = check && !(GetAsyncKeyState(VK_MENU)); }
		return check;
	}
	void RunSwitchHotkey()
	{
		while (*Check)
		{
			if (status == 1)
			{
				HotKeyfuntion();
			}
			if (status == -1)Sleep(50);
		}
	}
	void CheckSwitchHotkey1()
	{
		while (*Check)
		{
			bool KEY1 = GetKeyStatus(key1);
			if (KEY1)
			{
				status *= -1;
			}
			while (GetAsyncKeyState(key1))Sleep(25);
			Sleep(50);
		}
	}

	void CheckSwitchHotkey2()
	{
		while (*Check)
		{
			bool KEY1 = GetKeyStatus(key1);
			bool KEY2 = GetKeyStatus(key2);
			if (KEY1)
			{
				status = 1;
			}
			else if (KEY2)
			{
				status = -1;
			}
			while (GetAsyncKeyState(key1) || GetAsyncKeyState(key2))Sleep(25);
			Sleep(50);
		}
	}

	void RunOnceHotkey()
	{
		while (*Check)
		{
			if (GetKeyStatus(key1))
			{
				HotKeyfuntion();
			}
			while (GetAsyncKeyState(key1))Sleep(25);
			Sleep(50);
		}
	}

	void RunContinuousHotkey()
	{
		while (*Check)
		{
			while (GetKeyStatus(key1))
			{
				HotKeyfuntion();
			}
			Sleep(50);
		}
	}
};