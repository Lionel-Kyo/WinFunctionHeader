# WinHotkeyHeader

You should include this header to your C++ Project by

	#inclde "Hotkey.h"
	
This header have 3 classes: SwitchHotkey, OnceHotKey and ContinuousHotkey.
All classes need to pass at least 3 values: int vk_key, running function, break bool function.
In SwitchHotkey can passes int open_key, int off_key, running void function, break bool function.

Hotkey can be created by:

	SwitchHotkey* switchhotkey = new SwitchHotkey(97, []()
			{
				//script
			}, []()
			{
				return (bool)g_dlg->m_NumpadCheck.GetCheck();
			});

or

	void running()
	{
		sleep(25);
	}
	
	bool break()
	{
		return (bool)g_dlg->m_NumpadCheck.GetCheck();
	}


	OnceHotKey* oncehotkey = new OnceHotKey(97, running, break)

For running turn on the hotkey check just use:

	switchhotkey->run();
	or
	oncehotkey->run();
	
