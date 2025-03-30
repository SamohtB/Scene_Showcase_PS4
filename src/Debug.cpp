#include "Debug.h"

bool Debug::timestampEnabled = true;

// ANSI color codes
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

std::string Debug::GetTimestamp()
{
    std::time_t now = std::time(nullptr);
    struct tm dateTime;
    localtime_s(&dateTime, &now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "[%H:%M:%S]", &dateTime);
    return String(buffer);
}

void Debug::Log(const String& message)
{
    Print(message, "[LOG]", RESET);
}

void Debug::LogWarning(const String& message)
{
    Print(message, "[WARNING]", YELLOW);
}

void Debug::LogError(const String& message)
{
    Print(message, "[ERROR]", RED);
}

void Debug::Print(const String& message, const String& prefix, const String& colorCode)
{
    std::ostringstream output;

    if (timestampEnabled)
        output << GetTimestamp() << " ";

    output << prefix << " " << message;

    std::cout << colorCode << output.str() << RESET << std::endl;
}
