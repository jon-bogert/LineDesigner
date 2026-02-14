#pragma once

#include <string>
#include <sstream>

namespace Message
{
	void ErrorNotice(const std::string& msg);
	inline void ErrorNotice(const std::stringstream& msg) { ErrorNotice(msg.str()); };
	void InfoNotice(const std::string& msg);
	inline void InfoNotice(const std::stringstream& msg) { InfoNotice(msg.str()); };
}