#include "tests-base.hpp"

#include "utf8rewind.h"

#include "helpers-seeking.hpp"

TEST(Utf8SeekSet, OneByteSingle)
{
	const char* t = "v";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleFirst)
{
	const char* t = "\0";

	EXPECT_SEEKEQ("", 0, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleLast)
{
	const char* t = "\x7F";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleInvalidContinuationByteFirst)
{
	const char* t = "\x80";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleInvalidContinuationByteLast)
{
	const char* t = "\xBF";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleIllegalByteFirst)
{
	const char* t = "\xFE";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteSingleIllegalByteLast)
{
	const char* t = "\xFF";

	EXPECT_SEEKEQ("", 1, utf8seek(t, t, 1, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteMultiple)
{
	const char* t = "Miner";

	EXPECT_SEEKEQ("er", 3, utf8seek(t, t, 3, SEEK_SET));
}

TEST(Utf8SeekSet, OneByteMultipleInvalid)
{
	const char* t = "\x81\x99\xAD\x92\xBF\xFF\xB3";

	EXPECT_SEEKEQ("\xB3", 6, utf8seek(t, t, 6, SEEK_SET));
}

TEST(Utf8SeekSet, TwoBytes)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x430, o);
}

TEST(Utf8SeekSet, TwoBytesOverlong)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_SET);

	EXPECT_EQ(t + 2, r);
	EXPECT_STREQ("\xC1\xBF\xC0\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekSet, TwoBytesOverlongPastEnd)
{
	const char* t = "\xC0\xAF\xC1\xBF\xC0\xAF";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, TwoBytesLonelyStart)
{
	const char* t = "Gone\xC4" "Fishin'";

	const char* r = utf8seek(t, t, 8, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("hin'", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('h', o);
}

TEST(Utf8SeekSet, TwoBytesLonelyStartAtEnd)
{
	const char* t = "Megalodon\xDC";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, ThreeBytes)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("\xE0\xA4\xB4\xE0\xA4\xBD", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x934, o);
}

TEST(Utf8SeekSet, ThreeBytesOverlong)
{
	const char* t = "\xE0\x80\xAF\xE0\x9F\xBF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 1, SEEK_SET);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("\xE0\x9F\xBF\xE0\x80\xAF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekSet, ThreeBytesOverlongPastEnd)
{
	const char* t = "\xE0\x80\xAF\xE0\x80\xAF\xE0\x80\xAF";

	const char* r = utf8seek(t, t, 12, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, ThreeBytesLonelyStart)
{
	const char* t = "Rocker\xE4" "coaster";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("ster", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('s', o);
}

TEST(Utf8SeekSet, ThreeBytesLonelyStartAtEnd)
{
	const char* t = "Submarine\xED";

	const char* r = utf8seek(t, t, 10, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, ThreeBytesNotEnoughData)
{
	const char* t = "Compan\xEF\xBFions";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("ions", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('i', o);
}

TEST(Utf8SeekSet, ThreeBytesNotEnoughDataAtEnd)
{
	const char* t = "BBBark\xEF\xBF";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FourBytes)
{
	const char* t = "\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80\xF0\x90\x92\x80";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xF0\x90\x92\x80\xF0\x90\x92\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0x10480, o);
}

TEST(Utf8SeekSet, FourBytesOverlong)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("\xF4\x90\x80\x80", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekSet, FourBytesOverlongPastEnd)
{
	const char* t = "\xF0\x80\x80\xAF\xF0\x80\x80\xAF\xF7\xBF\xBF\xBF\xF4\x90\x80\x80";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 16, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FourBytesLonelyStart)
{
	const char* t = "Clam\xF4shellpower";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("hellpower", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('h', o);
}

TEST(Utf8SeekSet, FourBytesLonelyStartAtEnd)
{
	const char* t = "Magic\xF6";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FourBytesNotEnoughData)
{
	const char* t = "Brilli\xF0\x90\x80" "ant";

	const char* r = utf8seek(t, t, 8, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("nt", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('n', o);
}

TEST(Utf8SeekSet, FourBytesNotEnoughDataAtEnd)
{
	const char* t = "Night\xF0\x90\x80";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FiveBytesOverlong)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 10, r);
	EXPECT_STREQ("\xFB\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekSet, FiveBytesOverlongPastEnd)
{
	const char* t = "\xF8\x88\x80\x80\x80\xF8\x80\x80\x80\xAF\xFB\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 11, SEEK_SET);

	EXPECT_EQ(t + 15, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FiveBytesLonelyStart)
{
	const char* t = "Beyond\xFAMegalodon";

	const char* r = utf8seek(t, t, 7, SEEK_SET);

	EXPECT_EQ(t + 7, r);
	EXPECT_STREQ("Megalodon", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('M', o);
}

TEST(Utf8SeekSet, FiveBytesLonelyStartAtEnd)
{
	const char* t = "Brontos\xFA";

	const char* r = utf8seek(t, t, 8, SEEK_SET);

	EXPECT_EQ(t + 8, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, FiveBytesNotEnoughData)
{
	const char* t = "Central\xF8\x80\x80\x80Plaza baker";

	const char* r = utf8seek(t, t, 8, SEEK_SET);

	EXPECT_EQ(t + 11, r);
	EXPECT_STREQ("Plaza baker", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('P', o);
}

TEST(Utf8SeekSet, FiveBytesNotEnoughDataAtEnd)
{
	const char* t = "Apple" "\xF8\x80\x80\x80";

	const char* r = utf8seek(t + strlen(t), t, 6, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, SixBytesOverlong)
{
	const char* t = "\xFC\x84\x80\x80\x80\x80\xFC\x80\x80\x80\x80\x80\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF";

	const char* r = utf8seek(t, t, 2, SEEK_SET);

	EXPECT_EQ(t + 12, r);
	EXPECT_STREQ("\xFC\x84\x80\x80\x80\x80\xFD\xBF\xBF\xBF\xBF\xBF", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ(0xFFFD, o);
}

TEST(Utf8SeekSet, SixBytesLonelyStart)
{
	const char* t = "FC\xFCKnudde festival";

	const char* r = utf8seek(t, t, 3, SEEK_SET);

	EXPECT_EQ(t + 3, r);
	EXPECT_STREQ("Knudde festival", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('K', o);
}

TEST(Utf8SeekSet, SixBytesLonelyStartAtEnd)
{
	const char* t = "Stuff" "\xFD";

	const char* r = utf8seek(t, t, 6, SEEK_SET);

	EXPECT_EQ(t + 6, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, SixBytesNotEnoughData)
{
	const char* t = "Hint\xFC\x80\x80\x80\x80machine";

	const char* r = utf8seek(t, t, 5, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("machine", r);

	unicode_t o = 0;
	EXPECT_EQ(4, utf8toutf32(r, strlen(r), &o, sizeof(o), nullptr));
	EXPECT_EQ('m', o);
}

TEST(Utf8SeekSet, SixBytesNotEnoughDataAtEnd)
{
	const char* t = "Dull\xFC\x80\x80\x80\x80";

	const char* r = utf8seek(t, t, 5, SEEK_SET);

	EXPECT_EQ(t + 9, r);
	EXPECT_STREQ("", r);

	unicode_t o = 0;
	int32_t errors = UTF8_ERR_NONE;
	EXPECT_EQ(0, utf8toutf32(r, strlen(r), &o, sizeof(o), &errors));
	EXPECT_ERROREQ(UTF8_ERR_INVALID_DATA, errors);
}

TEST(Utf8SeekSet, StringPastEnd)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	EXPECT_SEEKEQ("", 22, utf8seek(t, t, 33, SEEK_SET));
}

TEST(Utf8SeekSet, StringAtEnd)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\xE0\xA4\xB4\xE0\xA4\xBD";

	EXPECT_SEEKEQ("\xE0\xA4\xB4\xE0\xA4\xBD", 6, utf8seek(t + strlen(t), t, 2, SEEK_SET));
}

TEST(Utf8SeekSet, StringFromMiddle)
{
	const char* t = "The Doctor";

	EXPECT_SEEKEQ("Doctor", 4, utf8seek(t, t, 4, SEEK_SET));
}

TEST(Utf8SeekSet, StringEndsInMiddle)
{
	const char* t = "\xE0\xA4\x81\xE0\xA4\x8B\0\xE0\xA4\xB4\xE0\xA4\xBD";

	EXPECT_SEEKEQ("", 6, utf8seek(t, t, 5, SEEK_SET));
}

TEST(Utf8SeekSet, StringZeroOffset)
{
	const char* t = "Magic powered";

	EXPECT_SEEKEQ("Magic powered", 0, utf8seek(t, t, 0, SEEK_SET));
}

TEST(Utf8SeekSet, StringNegativeOffset)
{
	const char* t = "Dreaming";

	EXPECT_SEEKEQ("Dreaming", 0, utf8seek(t, t, -12, SEEK_SET));
}

TEST(Utf8SeekSet, StringSwappedParameters)
{
	const char* t = "\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8";

	EXPECT_SEEKEQ("\xD0\xBB\xD0\xBE\xD0\xBA\xD0\xB0\xD0\xBB\xD0\xB8\xD0\xB7\xD0\xB0\xD1\x86\xD0\xB8\xD0\xB8", 0, utf8seek(t, t + strlen(t), 3, SEEK_SET));
}

TEST(Utf8SeekSet, StringEmpty)
{
	const char* t = "";

	EXPECT_SEEKEQ("", 0, utf8seek(t, t, 3, SEEK_SET));
}