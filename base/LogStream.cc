#include "LogStream.h"
#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>

using namespace daina;
using namespace daina::detail;

namespace daina {
namespace detail {
    const char digits[] = "9876543210123456789";
    const char* Zero    = digits + 9;
    static_assert(sizeof(digits) == 20, "数字数目错误");
    const char hexDigits[] = "0123456789ABCDEF";
    static_assert(sizeof(hexDigits) == 17, "16进制数字数目错误");

    /*
     * 广义整型转字符串
     */
    template <typename T>
    size_t convert(char buf[], T value)
    {
        T i     = value;
        char* p = buf;
        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = Zero[lsd];
        } while (i != 0);
        if (value < 0)
        {
            *p++ = '-';
        }
        *p = 0;
        std::reverse(buf, p);
        return p - buf;
    }
    size_t convertHex(char buf[], uintptr_t value)
    {
        uintptr_t i = value;
        char* p     = buf;
        do
        {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = hexDigits[lsd];
        } while (i != 0);
        *p = 0;
        std::reverse(buf, p);
        return p - buf;
    }
    template class LogBuffer<smallBufferSize>;
    template class LogBuffer<largeBufferSize>;
}  // namespace detail

string formatAsSI(int64_t n)
{
    char buf[10];
    if (n < 1000)
        snprintf(buf, sizeof buf, "%" PRId64, n);
    else if (n < 9995)
        snprintf(buf, sizeof buf, "%.2fK", n / 1e3);
    else if (n < 99950)
        snprintf(buf, sizeof buf, "%.1fK", n / 1e3);
    else if (n < 999500)
        snprintf(buf, sizeof buf, "%" PRId64 "K", n / 1000);
    else if (n < 9995000)
        snprintf(buf, sizeof buf, "%.2fM", n / 1e6);
    else if (n < 99950000)
        snprintf(buf, sizeof buf, "%.1fM", n / 1e6);
    else if (n < 999500000)
        snprintf(buf, sizeof buf, "%" PRId64 "M", n / 1000000);
    else if (n < 9995000000)
        snprintf(buf, sizeof buf, "%.2fG", n / 1e9);
    else if (n < 99950000000)
        snprintf(buf, sizeof buf, "%.1fG", n / 1e9);
    else if (n < 999500000000)
        snprintf(buf, sizeof buf, "%" PRId64 "G", n / 1000000000);
    else if (n < 9995000000000)
        snprintf(buf, sizeof buf, "%.2fT", n / 1e12);
    else if (n < 99950000000000)
        snprintf(buf, sizeof buf, "%.1fT", n / 1e12);
    else if (n < 999500000000000)
        snprintf(buf, sizeof buf, "%" PRId64 "T", n / 1000000000000);
    else if (n < 9995000000000000)
        snprintf(buf, sizeof buf, "%.2fP", n / 1e15);
    else if (n < 99950000000000000)
        snprintf(buf, sizeof buf, "%.1fP", n / 1e15);
    else if (n < 999500000000000000)
        snprintf(buf, sizeof buf, "%" PRId64 "T", n / 1000000000000000);
    else
        snprintf(buf, sizeof buf, "%.2fE", n / 1e18);
    return buf;
}

string formatAsICE(int64_t n)
{
    char buf[10];
    const double Ki = 1024.0;
    const double Mi = Ki * 1024.0;
    const double Gi = Mi * 1024.0;
    const double Ti = Gi * 1024.0;
    const double Pi = Ti * 1024.0;
    const double Ei = Ei * 1024.0;
    if (n < Ki)
        snprintf(buf, sizeof buf, "%" PRId64, n);
    else if (n < 9.995 * Ki)
        snprintf(buf, sizeof buf, "%.2f", n / Ki);
    else if (n < 99.95 * Ki)
        snprintf(buf, sizeof buf, "%.1f", n / Ki);
    else if (n < 1023.5 * Ki)
        snprintf(buf, sizeof buf, "%.0f", n / Ki);
    else if (n < Mi * 9.995)
        snprintf(buf, sizeof buf, "%.2fMi", n / Mi);
    else if (n < Mi * 99.95)
        snprintf(buf, sizeof buf, "%.1fMi", n / Mi);
    else if (n < Mi * 1023.5)
        snprintf(buf, sizeof buf, "%.0fMi", n / Mi);
    else if (n < Gi * 9.995)
        snprintf(buf, sizeof buf, "%.2fGi", n / Gi);
    else if (n < Gi * 99.95)
        snprintf(buf, sizeof buf, "%.1fGi", n / Gi);
    else if (n < Gi * 1023.5)
        snprintf(buf, sizeof buf, "%.0fGi", n / Gi);
    else if (n < Ti * 9.995)
        snprintf(buf, sizeof buf, "%.2fTi", n / Ti);
    else if (n < Ti * 99.95)
        snprintf(buf, sizeof buf, "%.1fTi", n / Ti);
    else if (n < Ti * 1023.5)
        snprintf(buf, sizeof buf, "%.0fTi", n / Ti);
    else if (n < Pi * 9.995)
        snprintf(buf, sizeof buf, "%.2fPi", n / Pi);
    else if (n < Pi * 99.95)
        snprintf(buf, sizeof buf, "%.1fPi", n / Pi);
    else if (n < Pi * 1023.5)
        snprintf(buf, sizeof buf, "%.0fPi", n / Pi);
    else if (n < Ei * 9.995)
        snprintf(buf, sizeof buf, "%.2fEi", n / Ei);
    else
        snprintf(buf, sizeof buf, "%.1fEi", n / Ei);

    return buf;
}
}  // namespace daina
template <size_t SIZE>
void LogBuffer<SIZE>::cookieStart()
{
}
template <size_t SIZE>
void LogBuffer<SIZE>::cookieEnd()
{
}

void LogStream::staticCheck()
{
    static_assert(maxNumbericSize - 10 > std::numeric_limits<double>::digits10, "maxNumbericSize is large enough");
    static_assert(maxNumbericSize - 10 > std::numeric_limits<long double>::digits10, "maxNumbericSize is large enough");
    static_assert(maxNumbericSize - 10 > std::numeric_limits<long>::digits10, "maxNumbericSize is large enough");
    static_assert(maxNumbericSize - 10 > std::numeric_limits<long long>::digits10, "maxNumbericSize is large enough");
}

template <typename T>
void LogStream::formatInteger(T v)
{
    if (_buffer.avail() >= maxNumbericSize)
    {
        size_t len = convert(_buffer.current(), v);
        _buffer.mvForward(len);
    }
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (_buffer.avail() >= maxNumbericSize)
    {
        char* buf  = _buffer.current();
        buf[0]     = '0';
        buf[1]     = 'x';
        size_t len = convertHex(buf + 2, v);
        _buffer.mvForward(len + 2);
    }
    return *this;
}

/*
 * snprintf 使用的是Dragon4 算法
 * TODO 以后可以尝试使用 Grisu算法来替换
 */
LogStream& LogStream::operator<<(double v)
{
    if (_buffer.avail() > maxNumbericSize)
    {
        int len = snprintf(_buffer.current(), maxNumbericSize, "%.12g", v);
        _buffer.mvForward(len);
    }
    return *this;
}
