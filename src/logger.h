#pragma once 

void LH_Log(const char* level, const char* format, ...);

#define LH_INFO(...) LH_Log("INFO", __VA_ARGS__)
#define LH_DEBUG(...) LH_Log("DEBUG", __VA_ARGS__)
#define LH_WARN(...) LH_Log("WARN", __VA_ARGS__)
#define LH_ERROR(...) LH_Log("ERROR", __VA_ARGS__)
