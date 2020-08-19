#include "CurrentThread.h"
#include <cxxabi.h>    // C++ 符号名解析为人类易读的
#include <execinfo.h>  // for backtrace
namespace daina {
namespace CurrentThread {
    __thread int cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength    = 6;
    __thread const char* t_threadName = "unknown";
    static_assert(std::is_same<int, pid_t>::value, "pid_t is not int");

    string stackTrace(bool demangle)
    {
        string stack;
        const int max_frames = 200;
        void* frame[max_frames];
        int nptr       = ::backtrace(frame, max_frames);
        char** symbols = ::backtrace_symbols(frame, nptr);
        if (symbols)
        {
            size_t len      = 256;
            char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
            for (int i = 1; i < nptr; ++i)  // i = 0 是本函数, 跳过
            {
                if (demangle)  // 符号转换
                {
                    char* left_par = nullptr;  // (
                    char* plus     = nullptr;  // +
                    for (char* p = symbols[i]; *p; ++p)
                    {
                        if (*p == '(')
                            left_par = p;
                        else if (*p == '+')
                            plus = p;
                    }
                    if (left_par && plus)
                    {
                        *plus        = 0;
                        int status   = 0;
                        char* result = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
                        *plus        = '+';
                        if (status == 0)
                        {
                            demangled = result;  // result could be realloc()
                            stack.append(symbols[i], left_par + 1);
                            stack.append(demangled);
                            stack.append(plus);
                            stack.push_back('\n');
                            continue;
                        }
                    }
                }
                stack.append(symbols[i]);
                stack.push_back('\n');
            }
        }
        return stack;
    }

}  // namespace CurrentThread
}  // namespace daina
