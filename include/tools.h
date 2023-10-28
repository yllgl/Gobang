#ifndef TOOLS_H
#define TOOLS_H
#include <memory>
#include <string>
#include <stdexcept>
template<class T>
inline T _max(const T& a, const T& b) {
	return (a > b) ? a : b;
}

template<class T>
inline T _min(const T& a, const T& b) {
	return (a < b) ? a : b;
}
template<class T>
inline T _abs(const T& a, const T& b) {
	return (a < 0) ? -a : a;
}
#define MAX(a,b) _max((a),(b))
#define MIN(a,b) _min((a),(b))
#define ABS(a) _abs((a))
static const int INF = 0x7ffffffe;
static const int WIN_MAX = 100000;
static const int WIN_MIN = 29000;
void SetWindowSize(int cols, int lines);
void SetCursorPosition(const int x, const int y);
void SetColor(int colorID);
void SetBackColor();
void OutputDebugPrintf(const char *strOutputString, ...);
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#endif // TOOLS_H