#include <rvi/base64.hpp>

#include <array>
#include <rvi/base.hpp>

#ifdef RVI_COMPILER_MSVC
	#pragma warning(disable: 26446)
	#pragma warning(disable: 26482)
	#pragma warning(disable: 26472)
	#pragma warning(disable: 26494)
#endif

void rvi::base64::encode(std::istream& in, std::ostringstream& out)
{
    std::array<unsigned char, 3> buff1;
    std::array<unsigned char, 4> buff2;

    uint8_t i = 0, j = 0;
    while (in.readsome(reinterpret_cast<char*>(&buff1[i++]), 1))
        if (i == 3)
        {
            out << ENCODE_TABLE[(buff1[0] & 0xfc) >> 2];
            out << ENCODE_TABLE[((buff1[0] & 0x03) << 4) + ((buff1[1] & 0xf0) >> 4)];
            out << ENCODE_TABLE[((buff1[1] & 0x0f) << 2) + ((buff1[2] & 0xc0) >> 6)];
            out << ENCODE_TABLE[buff1[2] & 0x3f];
            i = 0;
        }

    if (--i)
    {
        for (j = i; j < 3; j++) buff1[j] = '\0';

        buff2[0] = (buff1[0] & 0xfc) >> 2;
        buff2[1] = ((buff1[0] & 0x03) << 4) + ((buff1[1] & 0xf0) >> 4);
        buff2[2] = ((buff1[1] & 0x0f) << 2) + ((buff1[2] & 0xc0) >> 6);
        buff2[3] = buff1[2] & 0x3f;

        for (j = 0; j < (i + 1); j++)
        { 
            out << ENCODE_TABLE[buff2[j]];
        }

        while (i++ < 3) out << '=';
    }
}

void rvi::base64::decode(std::istringstream & in, std::ostream & out)
{
    std::array<unsigned char, 4> buff1;
    std::array<unsigned char, 4> buff2;

    uint8_t i = 0, j = 0;

    while (in.readsome(reinterpret_cast<char*>(&buff2[i]), 1) && buff2[i] != '=')
    {
        if (++i == 4)
        {
            for (i = 0; i != 4; i++)
                buff2[i] = DECODE_TABLE[buff2[i]];

            out << static_cast<char>((buff2[0] << 2) + ((buff2[1] & 0x30) >> 4));
            out << static_cast<char>(((buff2[1] & 0xf) << 4) + ((buff2[2] & 0x3c) >> 2));
            out << static_cast<char>(((buff2[2] & 0x3) << 6) + buff2[3]);

            i = 0;
        }
    }
    if (i)
    {
        for (j = i; j < 4; j++) buff2[j] = '\0';
        for (j = 0; j < 4; j++) buff2[j] = DECODE_TABLE[buff2[j]];

        buff1[0] = (buff2[0] << 2) + ((buff2[1] & 0x30) >> 4);
        buff1[1] = ((buff2[1] & 0xf) << 4) + ((buff2[2] & 0x3c) >> 2);
        buff1[2] = ((buff2[2] & 0x3) << 6) + buff2[3];

        for (j = 0; j < (i - 1); j++) out << static_cast<char>(buff1[j]);
    }
}