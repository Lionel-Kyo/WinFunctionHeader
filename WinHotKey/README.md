# WinHotkeyHeader

Link of example:
https://github.com/LionelC-Kz/CSO-Hotkey

This header should be included to your C++ Project by

	#inclde "Hotkey.h"
	
This header have 3 classes: SwitchHotkey, OnceHotKey and ContinuousHotkey.
All classes need to pass at least 3 values: int vk_key, running function, break bool function.
In SwitchHotkey can passes int open_key, int off_key, running void function, break bool function.

Hotkey can be created by:

	SwitchHotkey* switchhotkey = new SwitchHotkey(97, []()
			{
				//input your script
			}, []()
			{
				return (bool)g_dlg->m_NumpadCheck.GetCheck();
			});

or

	void running()
	{
		//input your script
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
	
