#include "tests-base.hpp"

#include "../helpers/helpers-locale.hpp"
#include "../helpers/helpers-strings.hpp"

class Utf8ToTitleTurkish
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		SET_LOCALE_TURKISH();
	}

	void TearDown()
	{
		RESET_LOCALE();
	}

};

TEST_F(Utf8ToTitleTurkish, SingleCapitalLetterI)
{
	// 0049
	// 0049

	const char* c = "I";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleCapitalLetterIAndDotAbove)
{
	// 0049 0307
	// 0049 0307

	const char* c = "I\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("I\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleCapitalLetterIWithDotAbove)
{
	// 0130
	// 0130

	const char* c = "\xC4\xB0";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB0", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleCapitalLetterIWithDotAboveAndDotAbove)
{
	// 0130 0307
	// 0130 0307

	const char* c = "\xC4\xB0\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB0\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleSmallLetterI)
{
	// 0069
	// 0130

	const char* c = "i";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(2, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB0", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleSmallLetterIWithDotAbove)
{
	// 0069 0307
	// 0130 0307

	const char* c = "i\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(4, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("\xC4\xB0\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleSmallLetterDotlessI)
{
	// 0131
	// 0049

	const char* c = "\xC4\xB1";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(1, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("I", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, SingleSmallLetterDotlessIWithDotAbove)
{
	// 0131 0307
	// 0049 0307

	const char* c = "\xC4\xB1\xCC\x87";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(3, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("I\xCC\x87", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}

TEST_F(Utf8ToTitleTurkish, WordCapitalLetterI)
{
	const char* c = "Imagine";
	const size_t s = 256;
	char b[s] = { 0 };
	int32_t errors = UTF8_ERR_NONE;

	EXPECT_EQ(7, utf8totitle(c, strlen(c), b, s - 1, &errors));
	EXPECT_UTF8EQ("Imagine", b);
	EXPECT_ERROREQ(UTF8_ERR_NONE, errors);
}