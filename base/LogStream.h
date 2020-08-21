#ifndef DAINA_LOGSTREAM_H
#define DAINA_LOGSTREAM_H
#include "Types.h"
#include "noncopyable.h"
#include <cassert>
#include <cstring>
namespace daina {
namespace detail {
    const size_t smallBufferSize = 4000;
    const size_t largeBufferSize = 4000 * 1000;
    template <size_t SIZE>
    class LogBuffer : noncopyable {
    public:
        LogBuffer() : _cur(_data)
        {
            setCookie(cookieStart);
        }
        ~LogBuffer()
        {
            setCookie(cookieEnd);
        }

        size_t avail() const
        {
            return end() - _cur;
        }

        void append(const char* buf, size_t len)
        {
            if (avail() > len)
            {
                memcpy(_cur, buf, len);
                _cur += len;
            }
        }

        const char* data() const
        {
            return _data;
        }

        size_t length() const
        {
            return _cur - _data;
        }

        char* current()
        {
            return _cur;
        }

        void mvForward(size_t offset)
        {
            _cur += offset;
        }

        void reset()
        {
            _cur = _data;
        }

        void bzero()
        {
            bZero(_data, sizeof(_data));
        }

        void setCookie(void (*cookie)())
        {
            _cookie = cookie;
        }

        string toString() const
        {
            return string(_data, length());
        }

    private:
        const char* end() const
        {
            return _data + sizeof(_data);
        }
        static void cookieStart();
        static void cookieEnd();

    private:
        void (*_cookie)();
        char _data[SIZE];
        char* _cur;
    };
}  // namespace detail
class LogStream : noncopyable {
    using self = LogStream;

public:
    using Buffer = detail::LogBuffer<detail::smallBufferSize>;
    self& operator<<(bool v)
    {
        _buffer.append(v ? "1" : "0", 1);
        return *this;
    }
    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);

    self& operator<<(const void*);
    self& operator<<(float f)
    {
        return *this << implicit_cast<double>(f);
    }
    self& operator<<(double);
    self& operator<<(char c)
    {
        _buffer.append(&c, 1);
        return *this;
    }
    self& operator<<(const char* str)
    {
        if (str)
        {
            _buffer.append(str, strlen(str));
        }
        else
        {
            _buffer.append("(NULL)", 6);
        }
        return *this;
    }
    self& operator<<(const unsigned char* str)
    {
        return *this << reinterpret_cast<const char*>(str);
    }
    self& operator<<(const string& str)
    {
        _buffer.append(str.c_str(), str.length());
        return *this;
    }
    self& operator<<(const Buffer& b)
    {
        return *this << b.toString();
    }

    void append(const char* data, size_t len)
    {
        _buffer.append(data, len);
    }

    const Buffer& buffer() const
    {
        return _buffer;
    }

    void resetBuffer()
    {
        _buffer.reset();
    }

private:
    void staticCheck();
    template <typename T>
    void formatInteger(T);

private:
    Buffer _buffer;
    static const int maxNumbericSize = 32;
};

string formatAsSI(int64_t);
string formatAsIEC(int64_t);
}  // namespace daina
#endif
