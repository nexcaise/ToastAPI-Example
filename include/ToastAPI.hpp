#pragma once

#include <string>

namespace nexcaise::toastapi {

bool sendToast(std::string title, std::string subtitle);
bool sendToastWithLink(std::string title, std::string subtitle, std::string link);
bool sendToastMessage(std::string message);

} // namespace nexcaise::toastapi