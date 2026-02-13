#include "Message.h"

#include <Windows.h>

void Message::ErrorNotice(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), "An Error has occured.", MB_ICONERROR | MB_OK);
}

void Message::InfoNotice(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), "Notice", MB_ICONINFORMATION | MB_OK);
}
