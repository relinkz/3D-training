#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <dinput.h>
#include "Engine.h"

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800



/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputHandler
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;

	unsigned char m_keyboardState[256];

public:
	InputHandler();
	virtual ~InputHandler();

	void initialize(HINSTANCE inst, HWND wnd);
	void shutdown();
	void frame();
	bool readKeyboard();

	bool qPressed() const;
	bool ePressed() const;
};

#endif

