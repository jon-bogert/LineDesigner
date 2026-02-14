#include "Message.h"

#include <Windows.h>

#ifdef _CONSOLE
#include <iostream>
#endif // _CONSOLE

void Message::ErrorNotice(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), "An Error has occured.", MB_ICONERROR | MB_OK);
}

void Message::InfoNotice(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), "Notice", MB_ICONINFORMATION | MB_OK);
}

void Message::DebugLog(const std::string& msg)
{
#ifdef _CONSOLE
	std::cout << msg << std::endl;
#endif // CONSOLE
}
