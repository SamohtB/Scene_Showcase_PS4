
#include <iostream>
#include <ctime>
#include <sstream>

using String = std::string;

class Debug
{
public:
    static void Log(const String& message);
    static void LogWarning(const String& message);
    static void LogError(const String& message);

private:
    static bool timestampEnabled;

    static String GetTimestamp();
    static void Print(const String& message, const String& prefix, const String& colorCode);
};


