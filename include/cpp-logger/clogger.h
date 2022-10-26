const int CPP_LOGGER_PRINT = 1;
const int CPP_LOGGER_ERROR = 2;
const int CPP_LOGGER_WARN = 3;
const int CPP_LOGGER_INFO = 4;
extern void clog(const int logger_level, const char* name, const char* string,
                 ...);