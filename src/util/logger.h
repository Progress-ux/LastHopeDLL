#ifdef __cplusplus
extern "C" {
#endif

void LH_LogInit(void);
void LH_LogShutdown(void);

void LH_Log(const char* level, const char* function, const char* format, ...);

#ifdef __cplusplus
}
#endif

#define LH_INFO(...)  LH_Log("INFO",  __func__, __VA_ARGS__)
#define LH_WARN(...)  LH_Log("WARN",  __func__, __VA_ARGS__)
#define LH_ERROR(...) LH_Log("ERROR", __func__, __VA_ARGS__)
#define LH_START(...) LH_Log("START", __func__, __VA_ARGS__)

#ifdef LH_DEBUG_LOG
#define LH_DEBUG(...) LH_Log("DEBUG", __func__, __VA_ARGS__)
#else
#define LH_DEBUG(...) ((void)0)
#endif
