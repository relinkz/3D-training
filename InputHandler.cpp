#include "InputHandler.h"



InputHandler::InputHandler()
{
	this->m_directInput = 0;
	this->m_keyboard = 0;
}


InputHandler::~InputHandler()
{
}

void InputHandler::initialize(HINSTANCE inst, HWND wnd)
{
	HRESULT result;

#pragma region initKeyboardInput
	// Initialize the main direct input interface.
	result = DirectInput8Create(inst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(wnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return;
	}
#pragma endregion
}

void InputHandler::shutdown()
{
	// Release the keyboard.
	if (this->m_keyboard)
	{
		this->m_keyboard->Unacquire();
		this->m_keyboard->Release();
		this->m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (this->m_directInput)
	{
		this->m_directInput->Release();
		this->m_directInput = 0;
	}
}

void InputHandler::frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = this->readKeyboard();
	if (!result)
	{
		return;
	}
	int i = 0;

	if (this->qPressed())
		i = 5;
	if (this->ePressed())
		i = 7;
}

bool InputHandler::readKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputHandler::qPressed() const
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;

	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_Q] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputHandler::ePressed() const
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_E] & 0x80)
	{
		return true;
	}

	return false;

}

