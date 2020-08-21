#include "base/LogStream.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

using daina::string;

TEST(LogStream, Booleans)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();
    EXPECT_EQ(buf.toString(), "");
    os << true;
    EXPECT_EQ(buf.toString(), "1");
    os << '\n';
    EXPECT_EQ(buf.toString(), "1\n");
    os << false;
    EXPECT_EQ(buf.toString(), "1\n0");
}

TEST(LogStream, Integers)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();
    EXPECT_EQ(buf.toString(), string(""));
    os << 1;
    EXPECT_EQ(buf.toString(), string("1"));
    os << 0;
    EXPECT_EQ(buf.toString(), string("10"));
    os << -1;
    EXPECT_EQ(buf.toString(), string("10-1"));
    os.resetBuffer();
    EXPECT_EQ(buf.toString(), string(""));
    os << 0 << " " << 123 << 'x' << 0x64;
    EXPECT_EQ(buf.toString(), string("0 123x100"));
}

TEST(LogStream, IntegerLimits)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();
    os << -2147483647;
    EXPECT_EQ(buf.toString(), "-2147483647");
    os << static_cast<int>(-2147483647 - 1);
    EXPECT_EQ(buf.toString(), "-2147483647-2147483648");
    os << ' ';
    os << 2147483647;
    EXPECT_EQ(buf.toString(), "-2147483647-2147483648 2147483647");
    os.resetBuffer();

    os << std::numeric_limits<int16_t>::min();
    EXPECT_EQ(buf.toString(), string("-32768"));
    os.resetBuffer();

    os << std::numeric_limits<int16_t>::max();
    EXPECT_EQ(buf.toString(), string("32767"));
    os.resetBuffer();

    os << std::numeric_limits<uint16_t>::min();
    EXPECT_EQ(buf.toString(), string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint16_t>::max();
    EXPECT_EQ(buf.toString(), string("65535"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::min();
    EXPECT_EQ(buf.toString(), string("-2147483648"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::max();
    EXPECT_EQ(buf.toString(), string("2147483647"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::min();
    EXPECT_EQ(buf.toString(), string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::max();
    EXPECT_EQ(buf.toString(), string("4294967295"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::min();
    EXPECT_EQ(buf.toString(), string("-9223372036854775808"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::max();
    EXPECT_EQ(buf.toString(), string("9223372036854775807"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::min();
    EXPECT_EQ(buf.toString(), string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::max();
    EXPECT_EQ(buf.toString(), string("18446744073709551615"));
    os.resetBuffer();

    int16_t a = 0;
    int32_t b = 0;
    int64_t c = 0;
    os << a;
    os << b;
    os << c;
    EXPECT_EQ(buf.toString(), string("000"));
}

TEST(LogStream, Floats)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();

    os << 0.0;
    EXPECT_EQ(buf.toString(), string("0"));
    os.resetBuffer();

    os << 1.0;
    EXPECT_EQ(buf.toString(), string("1"));
    os.resetBuffer();

    os << 0.1;
    EXPECT_EQ(buf.toString(), string("0.1"));
    os.resetBuffer();

    os << 0.05;
    EXPECT_EQ(buf.toString(), string("0.05"));
    os.resetBuffer();

    os << 0.15;
    EXPECT_EQ(buf.toString(), string("0.15"));
    os.resetBuffer();

    double a = 0.1;
    os << a;
    EXPECT_EQ(buf.toString(), string("0.1"));
    os.resetBuffer();

    double b = 0.05;
    os << b;
    EXPECT_EQ(buf.toString(), string("0.05"));
    os.resetBuffer();

    double c = 0.15;
    os << c;
    EXPECT_EQ(buf.toString(), string("0.15"));
    os.resetBuffer();

    os << a + b;
    EXPECT_EQ(buf.toString(), string("0.15"));
    os.resetBuffer();

    EXPECT_TRUE(a + b != c);

    os << 1.23456789;
    EXPECT_EQ(buf.toString(), string("1.23456789"));
    os.resetBuffer();

    os << 1.234567;
    EXPECT_EQ(buf.toString(), string("1.234567"));
    os.resetBuffer();

    os << -123.456;
    EXPECT_EQ(buf.toString(), string("-123.456"));
    os.resetBuffer();
}

TEST(LogStream, Void)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();

    os << static_cast<void*>(0);
    EXPECT_EQ(buf.toString(), string("0x0"));
    os.resetBuffer();

    os << reinterpret_cast<void*>(8888);
    EXPECT_EQ(buf.toString(), string("0x22B8"));
    os.resetBuffer();
}

TEST(LogStream, String)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();

    os << "Hello ";
    EXPECT_EQ(buf.toString(), string("Hello "));

    string St = "World!";
    os << St;
    EXPECT_EQ(buf.toString(), string("Hello World!"));
}

TEST(LogStream, LongBuffer)
{
    daina::LogStream os;
    const daina::LogStream::Buffer& buf = os.buffer();
    for (int i = 0; i < 399; ++i)
    {
        os << "123456789 ";
        EXPECT_EQ(buf.length(), 10 * (i + 1));
        EXPECT_EQ(buf.avail(), daina::detail::smallBufferSize - 10 * (i + 1));
    }

    os << "abcdefghi ";
    EXPECT_EQ(buf.length(), 3990);
    EXPECT_EQ(buf.avail(), daina::detail::smallBufferSize - 3990);

    os << "abcdefghi";
    EXPECT_EQ(buf.length(), 3999);
    EXPECT_EQ(buf.avail(), daina::detail::smallBufferSize - 3999);
}

TEST(LogStream, FormatSI)
{
    EXPECT_EQ(daina::formatAsSI(0), string("0"));
    EXPECT_EQ(daina::formatAsSI(999), string("999"));
    EXPECT_EQ(daina::formatAsSI(1000), string("1.00K"));
    EXPECT_EQ(daina::formatAsSI(9990), string("9.99K"));
    EXPECT_EQ(daina::formatAsSI(9994), string("9.99K"));
    EXPECT_EQ(daina::formatAsSI(9995), string("10.0K"));
    EXPECT_EQ(daina::formatAsSI(10000), string("10.0K"));
    EXPECT_EQ(daina::formatAsSI(10049), string("10.0K"));
    EXPECT_EQ(daina::formatAsSI(10050), string("10.1K"));
    EXPECT_EQ(daina::formatAsSI(99900), string("99.9K"));
    EXPECT_EQ(daina::formatAsSI(99949), string("99.9K"));
    EXPECT_EQ(daina::formatAsSI(99950), string("100K"));
    EXPECT_EQ(daina::formatAsSI(100499), string("100K"));
    // FIXME:
    EXPECT_EQ(daina::formatAsSI(100500), string("101K"));
    EXPECT_EQ(daina::formatAsSI(100501), string("101K"));
    EXPECT_EQ(daina::formatAsSI(999499), string("999K"));
    EXPECT_EQ(daina::formatAsSI(999500), string("1.00M"));
    EXPECT_EQ(daina::formatAsSI(1004999), string("1.00M"));
    EXPECT_EQ(daina::formatAsSI(1005000), string("1.01M"));
    EXPECT_EQ(daina::formatAsSI(1005001), string("1.01M"));
    EXPECT_EQ(daina::formatAsSI(INT64_MAX), string("9.22E"));
}
