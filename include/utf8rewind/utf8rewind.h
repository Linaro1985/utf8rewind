/*
	Copyright (C) 2014-2015 Quinten Lansu

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _UTF8REWIND_H_
#define _UTF8REWIND_H_

/*!
	\file
	\brief Public interface for UTF-8 functions.
*/

#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

/*!
	\defgroup version Version information
	\{
*/

/*!
	\def UTF8_VERSION_MAKE
	\brief Macro for creating a version number from a major, minor and bugfix
	number.
*/
#define UTF8_VERSION_MAKE(_major, _minor, _bugfix) \
	((_major) * 10000) + ((_minor) * 100) + (_bugfix)

/*!
	\def UTF8_VERSION_MAJOR
	\brief The major version number of this release.
*/
#define UTF8_VERSION_MAJOR   1

/*!
	\def UTF8_VERSION_MINOR
	\brief The minor version number of this release.
*/
#define UTF8_VERSION_MINOR   4

/*!
	\def UTF8_VERSION_BUGFIX
	\brief The bugfix version number of this release.
*/
#define UTF8_VERSION_BUGFIX  0

/*!
	\def UTF8_VERSION
	\brief The version number as an integer.
*/
#define UTF8_VERSION \
	UTF8_VERSION_MAKE(UTF8_VERSION_MAJOR, UTF8_VERSION_MINOR, UTF8_VERSION_BUGFIX)

/*!
	\def UTF8_VERSION_STRING
	\brief The verion number as a string.
*/
#define UTF8_VERSION_STRING  "1.4.0"

/*!
	\def UTF8_VERSION_GUARD
	\brief Check if feature is supported by the current release.
*/
#define UTF8_VERSION_GUARD(_major, _minor, _bugfix) \
	(UTF8_VERSION >= UTF8_VERSION_MAKE(_major, _minor, _bugfix))

/*!
	\}
*/

/*!
	\defgroup errors Error codes
	\{
*/

/*!
	\def UTF8_ERR_NONE
	\brief No errors.
*/
#define UTF8_ERR_NONE                           (0)

/*!
	\def UTF8_ERR_INVALID_DATA
	\brief Input data is invalid.
*/
#define UTF8_ERR_INVALID_DATA                   (-1)

/*!
	\def UTF8_ERR_INVALID_FLAG
	\brief Input flag is invalid.
*/
#define UTF8_ERR_INVALID_FLAG                   (-2)

/*!
	\def UTF8_ERR_NOT_ENOUGH_SPACE
	\brief Not enough space in buffer to store result.
*/
#define UTF8_ERR_NOT_ENOUGH_SPACE               (-3)

/*!
	\def UTF8_ERR_OVERLAPPING_PARAMETERS
	\brief Input and output buffers overlap in memory.
*/
#define UTF8_ERR_OVERLAPPING_PARAMETERS         (-4)

/*!
	\}
*/

#define UTF8_CATEGORY_LETTER_UPPERCASE          0x00000001
#define UTF8_CATEGORY_LETTER_LOWERCASE          0x00000002
#define UTF8_CATEGORY_LETTER_TITLECASE          0x00000004
#define UTF8_CATEGORY_LETTER_MODIFIER           0x00000008
#define UTF8_CATEGORY_LETTER_OTHER              0x00000010

#define UTF8_CATEGORY_LETTER \
	UTF8_CATEGORY_LETTER_UPPERCASE | UTF8_CATEGORY_LETTER_LOWERCASE | \
	UTF8_CATEGORY_LETTER_TITLECASE | UTF8_CATEGORY_LETTER_MODIFIER | \
	UTF8_CATEGORY_LETTER_OTHER

#define UTF8_CATEGORY_CASE_MAPPED \
	UTF8_CATEGORY_LETTER_UPPERCASE | UTF8_CATEGORY_LETTER_LOWERCASE | \
	UTF8_CATEGORY_LETTER_TITLECASE

#define UTF8_CATEGORY_MARK_NON_SPACING          0x00000020
#define UTF8_CATEGORY_MARK_SPACING              0x00000040
#define UTF8_CATEGORY_MARK_ENCLOSING            0x00000080

#define UTF8_CATEGORY_MARK \
	UTF8_CATEGORY_MARK_NON_SPACING | UTF8_CATEGORY_MARK_SPACING | \
	UTF8_CATEGORY_MARK_ENCLOSING

#define UTF8_CATEGORY_NUMBER_DECIMAL            0x00000100
#define UTF8_CATEGORY_NUMBER_LETTER             0x00000200
#define UTF8_CATEGORY_NUMBER_OTHER              0x00000400

#define UTF8_CATEGORY_NUMBER \
	UTF8_CATEGORY_NUMBER_DECIMAL | UTF8_CATEGORY_NUMBER_LETTER | \
	UTF8_CATEGORY_NUMBER_OTHER

#define UTF8_CATEGORY_PUNCTUATION_CONNECTOR     0x00000800
#define UTF8_CATEGORY_PUNCTUATION_DASH          0x00001000
#define UTF8_CATEGORY_PUNCTUATION_OPEN          0x00002000
#define UTF8_CATEGORY_PUNCTUATION_CLOSE         0x00004000
#define UTF8_CATEGORY_PUNCTUATION_INITIAL       0x00008000
#define UTF8_CATEGORY_PUNCTUATION_FINAL         0x00010000
#define UTF8_CATEGORY_PUNCTUATION_OTHER         0x00020000

#define UTF8_CATEGORY_PUNCTUATION \
	UTF8_CATEGORY_PUNCTUATION_CONNECTOR | UTF8_CATEGORY_PUNCTUATION_DASH | \
	UTF8_CATEGORY_PUNCTUATION_OPEN | UTF8_CATEGORY_PUNCTUATION_CLOSE | \
	UTF8_CATEGORY_PUNCTUATION_INITIAL | UTF8_CATEGORY_PUNCTUATION_FINAL | \
	UTF8_CATEGORY_PUNCTUATION_OTHER

#define UTF8_CATEGORY_SYMBOL_MATH               0x00040000
#define UTF8_CATEGORY_SYMBOL_CURRENCY           0x00080000
#define UTF8_CATEGORY_SYMBOL_MODIFIER           0x00100000
#define UTF8_CATEGORY_SYMBOL_OTHER              0x00200000

#define UTF8_CATEGORY_SYMBOL \
	UTF8_CATEGORY_SYMBOL_MATH | UTF8_CATEGORY_SYMBOL_CURRENCY | \
	UTF8_CATEGORY_SYMBOL_MODIFIER | UTF8_CATEGORY_SYMBOL_OTHER

#define UTF8_CATEGORY_SEPARATOR_SPACE           0x00400000
#define UTF8_CATEGORY_SEPARATOR_LINE            0x00800000
#define UTF8_CATEGORY_SEPARATOR_PARAGRAPH       0x01000000

#define UTF8_CATEGORY_SEPARATOR \
	UTF8_CATEGORY_SEPARATOR_SPACE | UTF8_CATEGORY_SEPARATOR_LINE | \
	UTF8_CATEGORY_SEPARATOR_PARAGRAPH

#define UTF8_CATEGORY_CONTROL                   0x02000000
#define UTF8_CATEGORY_FORMAT                    0x04000000
#define UTF8_CATEGORY_SURROGATE                 0x08000000
#define UTF8_CATEGORY_PRIVATE_USE               0x10000000
#define UTF8_CATEGORY_UNASSIGNED                0x20000000

#define UTF8_CATEGORY_ISCNTRL \
	UTF8_CATEGORY_CONTROL

#define UTF8_CATEGORY_ISPRINT \
	UTF8_CATEGORY_LETTER | UTF8_CATEGORY_NUMBER | \
	UTF8_CATEGORY_PUNCTUATION | UTF8_CATEGORY_SYMBOL | \
	UTF8_CATEGORY_SEPARATOR

#define UTF8_CATEGORY_ISSPACE \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISBLANK \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISGRAPH \
	UTF8_CATEGORY_LETTER | UTF8_CATEGORY_NUMBER | \
	UTF8_CATEGORY_PUNCTUATION | UTF8_CATEGORY_SYMBOL

#define UTF8_CATEGORY_ISPUNCT \
	UTF8_CATEGORY_PUNCTUATION | UTF8_CATEGORY_SYMBOL

#define UTF8_CATEGORY_ISALNUM \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISALPHA \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISUPPER \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISLOWER \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISDIGIT \
	UTF8_CATEGORY_SEPARATOR_SPACE

#define UTF8_CATEGORY_ISXDIGIT \
	UTF8_CATEGORY_SEPARATOR_SPACE

/*!
	\def UTF8_NORMALIZE_COMPOSE
	\brief Normalize input to Normalization Form C (NFC).
*/
#define UTF8_NORMALIZE_COMPOSE                  0x00000001

/*!
	\def UTF8_NORMALIZE_DECOMPOSE
	\brief Normalize input to Normalization Form D (NFD).
*/
#define UTF8_NORMALIZE_DECOMPOSE                0x00000002

/*!
	\def UTF8_NORMALIZE_COMPATIBILITY
	\brief Change Normalization Form from NFC to NFKC or from NFD to NFKD.
*/
#define UTF8_NORMALIZE_COMPATIBILITY            0x00000004

/*!
	\def UTF8_NORMALIZATION_RESULT_YES
	\brief Text is stable and does not have to be normalized.
*/
#define UTF8_NORMALIZATION_RESULT_YES           (0)

/*!
	\def UTF8_NORMALIZATION_RESULT_MAYBE
	\brief Text is unstable, but normalization may be skipped.
*/
#define UTF8_NORMALIZATION_RESULT_MAYBE         (1)

/*!
	\def UTF8_NORMALIZATION_RESULT_NO
	\brief Text is unstable and must be normalized.
*/
#define UTF8_NORMALIZATION_RESULT_NO            (2)

/*!
	\defgroup configuration Global configuration
	\{
*/

/*!
	\def UTF8_WCHAR_SIZE
	\brief Specifies the size of the `wchar_t` type. On Windows this is 2, on
	POSIX systems it is 4. If not specified on the command line, the compiler
	tries to automatically determine the size of the `wchar_t` type based on the
	environment.
*/

#ifndef UTF8_WCHAR_SIZE
	#if (__SIZEOF_WCHAR_T__ == 4) || (WCHAR_MAX > UINT16_MAX) || (__WCHAR_MAX__ > UINT16_MAX)
		#define UTF8_WCHAR_SIZE (4)
	#else
		#define UTF8_WCHAR_SIZE (2)
	#endif
#endif

#if (UTF8_WCHAR_SIZE == 4)
	/*!
		\def UTF8_WCHAR_UTF32
		\brief The `wchar_t` type is treated as UTF-32 (4 bytes).
	*/
	#define UTF8_WCHAR_UTF32 (1)
#elif (UTF8_WCHAR_SIZE == 2)
	/*!
		\def UTF8_WCHAR_UTF16
		\brief The `wchar_t` type is treated as UTF-16 (2 bytes).
	*/
	#define UTF8_WCHAR_UTF16 (1)
#else
	#error Invalid size for wchar_t type.
#endif

/*!
	\def UTF8_API
	\brief Calling convention for public functions.
*/

#ifndef UTF8_API
	#ifdef __cplusplus
		#define UTF8_API extern "C"
	#else
		#define UTF8_API
	#endif
#endif

/*!
	\}
*/

/*!
	\var utf16_t
	\brief UTF-16 encoded code point.
*/
typedef uint16_t utf16_t;

/*!
	\var unicode_t
	\brief UTF-32 encoded code point.
*/
typedef uint32_t unicode_t;

/*!
	\brief Get the length in code points of a UTF-8 encoded string.

	Example:

	\code{.c}
		uint8_t CheckPassword(const char* password)
		{
			size_t length = utf8len(password);
			return (length == utf8len("hunter2"));
		}
	\endcode

	\param[in]  text  UTF-8 encoded string.

	\return Length in code points.
*/
UTF8_API size_t utf8len(const char* text);

/*!
	\brief Convert a UTF-16 encoded string to a UTF-8 encoded string.

	\note This function should only be called directly if you are positive that
	you are working with UTF-16 encoded text. If you're working with wide
	strings, take a look at #widetoutf8 instead.

	Example:

	\code{.c}
		uint8_t Player_SetNameUtf16(const utf16_t* name, size_t nameSize)
		{
			char buffer[256];
			size_t buffer_size = 255;
			size_t converted_size;
			int32_t errors;

			converted_size = utf16toutf8(name, nameSize, buffer, buffer_size, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				return 0;
			}
			buffer[converted_size] = 0;

			return Player_SetName(converted_name);
		}
	\endcode

	\param[in]   input       UTF-16 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf32toutf8
	\sa widetoutf8
*/
UTF8_API size_t utf16toutf8(const utf16_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-32 encoded string to a UTF-8 encoded string.

	\note This function should only be called directly if you are positive that
	you are working with UTF-32 encoded text. If you're working with wide
	strings, take a look at #widetoutf8 instead.

	Example:

	\code{.c}
		uint8_t Database_ExecuteQuery_Unicode(const unicode_t* query, size_t querySize)
		{
			char* converted = NULL;
			size_t converted_size;
			uint8_t result = 0;
			int32_t errors;
			
			converted_size = utf32toutf8(query, querySize, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			utf32toutf8(query, querySize, converted, converted_size, NULL);
			converted[converted_size] = 0;

			result = Database_ExecuteQuery(converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = 0;
			}

			return result;
		}
	\endcode

	\param[in]   input       UTF-32 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf16toutf8
	\sa widetoutf8
*/
UTF8_API size_t utf32toutf8(const unicode_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a wide string to a UTF-8 encoded string.

	Depending on the platform, wide strings are either UTF-16 or UTF-32 encoded.
	This function takes a wide string as input and automatically calls the
	correct conversion function.
	
	This allows for a cross-platform treatment of wide text and is preferable to
	using the UTF-16 or UTF-32 versions directly.

	Example:

	\code{.c}
		texture_t Texture_Load_Wide(const wchar_t* input)
		{
			char* converted = NULL;
			size_t converted_size;
			size_t input_size = wcslen(input) * sizeof(wchar_t);
			texture_t result = NULL;
			int32_t errors;

			converted_size = widetoutf8(input, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			widetoutf8(input, input_size, converted, converted_size, NULL);
			converted[converted_size / sizeof(wchar_t)] = 0;

			result = Texture_Load(converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}

			return result;
		}
	\endcode

	\param[in]   input       Wide-encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8towide
	\sa utf16toutf8
	\sa utf32toutf8
*/
UTF8_API size_t widetoutf8(const wchar_t* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a UTF-16 encoded string.

	\note This function should only be called directly if you are positive that
	you *must* convert to UTF-16, independent of platform. If you're working
	with wide strings, take a look at #utf8towide instead.

	Erroneous byte sequences such as missing or illegal bytes or overlong
	encoding of code points (e.g. using five bytes to encode a sequence that
	can be represented by two bytes) are converted to the replacement
	character U+FFFD.

	Example:

	\code{.c}
		void Font_DrawText(int x, int y, const char* text)
		{
			utf16_t buffer[256];
			size_t buffer_size = 255 * sizeof(utf16_t);
			int32_t errors;
			
			size_t converted_size = utf8toutf16(text, strlen(text), buffer, buffer_size, &errors);
			if (converted_size > 0 &&
				errors == UTF8_ERR_NONE)
			{
				Legacy_DrawText(g_FontCurrent, x, y, (unsigned short*)buffer, converted_size / sizeof(utf16_t));
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8towide
	\sa utf8toutf32
*/
UTF8_API size_t utf8toutf16(const char* input, size_t inputSize, utf16_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a UTF-32 encoded string.

	\note This function should only be called directly if you are positive that
	you *must* convert to UTF-32, independent of platform. If you're working
	with wide strings, take a look at #utf8towide instead.

	Erroneous byte sequences such as missing or illegal bytes or overlong
	encoding of code points (e.g. using five bytes to encode a sequence that
	can be represented by two bytes) are converted to the replacement
	character U+FFFD.

	Example:

	\code{.c}
		void TextField_AddCharacter(const char* encoded)
		{
			unicode_t code_point = 0;
			int32_t errors;

			utf8toutf32(encoded, strlen(encoded), &code_point, sizeof(unicode_t), &errors);
			if (errors == UTF8_ERR_NONE)
			{
				TextField_AddCodePoint(code_point);
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8towide
	\sa utf8toutf16
*/
UTF8_API size_t utf8toutf32(const char* input, size_t inputSize, unicode_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert a UTF-8 encoded string to a wide string.

	Depending on the platform, wide strings are either UTF-16 or UTF-32 encoded.
	This function takes a UTF-8 encoded string as input and automatically calls
	the correct conversion function.

	This allows for a cross-platform treatment of wide text and is preferable to
	using the UTF-16 or UTF-32 versions directly.

	Erroneous byte sequences such as missing or illegal bytes or overlong
	encoding of code points (e.g. using five bytes to encode a sequence that
	can be represented by two bytes) are converted to the replacement
	character U+FFFD.

	\note Code points outside the Basic Multilingual Plane (BMP) are converted
	to surrogate pairs when using UTF-16. This means that strings containing
	characters outside the BMP converted on a platform with UTF-32 wide strings
	are *not* compatible with platforms with UTF-16 wide strings.

	\par Hence, it is preferable to store all data as UTF-8 and only convert to
	wide strings when required by a third-party interface.

	Example:

	\code{.c}
		void Window_SetTitle(void* windowHandle, const char* text)
		{
			size_t input_size = strlen(text);
			wchar_t* converted = NULL;
			size_t converted_size;
			int32_t errors;

			converted_size = utf8towide(text, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (wchar_t*)malloc(converted_size + sizeof(wchar_t));
			utf8towide(text, input_size, converted, converted_size, NULL);
			converted[converted_size / sizeof(wchar_t)] = 0;

			SetWindowTextW((HWND)windowHandle, converted);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa widetoutf8
	\sa utf8toutf16
	\sa utf8toutf32
*/
UTF8_API size_t utf8towide(const char* input, size_t inputSize, wchar_t* target, size_t targetSize, int32_t* errors);

/*!
	\brief Seek into a UTF-8 encoded string.

	Working with UTF-8 encoded strings can be tricky due to the nature of the
	variable-length encoding. Because one character no longer equals one byte,
	it can be difficult to skip around in a UTF-8 encoded string without
	decoding the code points.

	This function provides an interface similar to `fseek` in order to enable
	skipping to another part of the string.

	\note `textStart` must come before `text` in memory when seeking from the
	current or end position.

	Example:

	\code{.c}
		const char* text = "Press \xE0\x80\x13 to continue.";
		const char fixed[1024];
		const char* commandStart;
		const char* commandEnd;

		memset(fixed, 0, sizeof(fixed));

		commandStart = strstr(text, "\xE0\x80\x13");
		if (commandStart == 0)
		{
			return 0;
		}

		strncpy(fixed, text, commandStart - text);
		strcat(fixed, "ENTER");

		commandEnd = utf8seek(commandStart, strlen(commandStart), text, 1, SEEK_CUR);
		if (commandEnd != commandStart)
		{
			strcat(fixed, commandEnd);
		}
	\endcode

	\param[in]  text       Input string.
	\param[in]  textSize   Size of input string in bytes.
	\param[in]  textStart  Start of input string.
	\param[in]  offset     Requested offset in code points.
	\param[in]  direction  Direction to seek in.
	\arg `SEEK_SET` Offset is from the start of the string.
	\arg `SEEK_CUR` Offset is from the current position of the string.
	\arg `SEEK_END` Offset is from the end of the string.

	\return Pointer to offset string or no change on error.
*/
UTF8_API const char* utf8seek(const char* text, size_t textSize, const char* textStart, off_t offset, int direction);

/*!
	\brief Convert UTF-8 encoded text to uppercase.

	This function allows conversion of UTF-8 encoded strings to uppercase
	without first changing the encoding to UTF-32. Conversion is fully compliant
	with the Unicode 7.0 standard.

	Although most code points can be converted in-place, there are notable
	exceptions. For example, U+00DF (LATIN SMALL LETTER SHARP S) maps to
	"U+0053 U+0053" (LATIN CAPITAL LETTER S and LATIN CAPITAL LETTER S) when
	converted to uppercase. Therefor, it is advised to first determine the size
	in bytes of the output by calling the function with a NULL output buffer.

	Only a handful of scripts make a distinction between upper and lowercase.
	In addition to modern scripts, such as Latin, Greek, Armenian and Cyrillic,
	a few historic or archaic scripts have case. The vast majority of scripts
	do not have case distinctions.

	\note Case mapping is not reversible. That is, `toUpper(toLower(x))
	!= toLower(toUpper(x))`.

	\note This function checks the (thread-local) system locale in order to
	support languages with exceptional behavior on specific code points.
	Unfortunately, no cross-platform way of setting and retrieving the system
	locale is available without adding dependencies to the library. Please
	refer to your operating system's manual to see how to setup the system
	locale on your target system.
	
	\par For more information on these exceptional code points, please refer
	to the text file made available by the Unicode Consortium:
	ftp://ftp.unicode.org/Public/UNIDATA/SpecialCasing.txt

	Example:

	\code{.c}
		void Button_Draw(int32_t x, int32_t y, const char* text)
		{
			size_t input_size = strlen(text);
			char* converted = NULL;
			size_t converted_size;
			int32_t text_box_width, text_box_height;
			int32_t errors;

			converted_size = utf8toupper(text, input_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			utf8toupper(text, input_size, converted, converted_size, NULL);
			converted[converted_size] = 0;

			Font_GetTextDimensions(converted, &text_box_width, &text_box_height);

			Draw_BoxFilled(x - 4, y - 4, text_box_width + 8, text_box_height + 8, 0x088A08);
			Draw_BoxOutline(x - 4, y - 4, text_box_width + 8, text_box_height + 8, 0xA9F5A9);
			Font_DrawText(x + 2, y + 1, converted, 0x000000);
			Font_DrawText(x, y, converted, 0xFFFFFF);

		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed to contain output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8tolower
	\sa utf8totitle
	\sa utf8casefold
*/
UTF8_API size_t utf8toupper(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert UTF-8 encoded text to lowercase.

	This function allows conversion of UTF-8 encoded strings to lowercase
	without first changing the encoding to UTF-32. Conversion is fully compliant
	with the Unicode 7.0 standard.

	Although most code points can be converted to lowercase in-place, there are
	notable exceptions. For example, U+0130 (LATIN CAPITAL LETTER I WITH DOT
	ABOVE) maps to "U+0069 U+0307" (LATIN SMALL LETTER I and COMBINING DOT
	ABOVE) when converted to lowercase. Therefor, it is advised to first
	determine the size in bytes of the output by calling the function with a
	NULL output buffer.

	Only a handful of scripts make a distinction between upper- and lowercase.
	In addition to modern scripts, such as Latin, Greek, Armenian and Cyrillic,
	a few historic or archaic scripts have case. The vast majority of scripts do
	not have case distinctions.

	\note Case mapping is not reversible. That is, `toUpper(toLower(x))
	!= toLower(toUpper(x))`.

	\note This function checks the (thread-local) system locale in order to
	support languages with exceptional behavior on specific code points.
	Unfortunately, no cross-platform way of setting and retrieving the system
	locale is available without adding dependencies to the library. Please
	refer to your operating system's manual to see how to setup the system
	locale on your target system.

	\par For more information on these exceptional code points, please refer
	to the text file made available by the Unicode Consortium:
	ftp://ftp.unicode.org/Public/UNIDATA/SpecialCasing.txt

	Example:

	\code{.c}
		author_t* Author_ByName(const char* name)
		{
			author_t* result = NULL;
			size_t name_size = strlen(name);
			char* converted = NULL;
			size_t converted_size;
			int32_t errors;
			size_t i;
			
			converted_size = utf8tolower(name, name_size, NULL, 0, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				goto cleanup;
			}

			converted = (char*)malloc(converted_size + 1);
			utf8tolower(name, name_size, converted, converted_size, NULL);
			converted[converted_size] = 0;
			
			for (i = 0; i < g_AuthorCount; ++i)
			{
				if (!strcmp(g_Author[i].name, converted))
				{
					result = &g_Author[i];
					break;
				}
			}
		
		cleanup:
			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
			return result;
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8toupper
	\sa utf8totitle
	\sa utf8casefold
*/
UTF8_API size_t utf8tolower(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Convert UTF-8 encoded text to titlecase.

	This function allows conversion of UTF-8 encoded strings to titlecase
	without first changing the encoding to UTF-32. Conversion is fully compliant
	with the Unicode 7.0 standard.

	Titlecase requires a bit more explanation than uppercase and lowercase,
	because it is not a common text transformation. Titlecase uses uppercase
	for the first letter of each word and lowercase for the rest. Words are
	defined as "collections of code points with general category Lu, Ll, Lt, Lm
	or Lo according to the Unicode database".

	Effectively, any type of punctuation can break up a word, even if this is
	not grammatically valid. This happens because the titlecasing algorithm
	does not and cannot take grammar rules into account.

	Text                                 | Titlecase
	-------------------------------------|-------------------------------------
	The running man                      | The Running Man
	NATO Alliance                        | Nato Alliance
	You're amazing at building libraries | You'Re Amazing At Building Libraries
	
	Although most code points can be converted to titlecase in-place, there are
	notable exceptions. For example, U+00DF (LATIN SMALL LETTER SHARP S) maps to
	"U+0053 U+0073" (LATIN CAPITAL LETTER S and LATIN SMALL LETTER S) when
	converted to titlecase. Therefor, it is advised to first determine the size
	in bytes of the output by calling the function with a NULL output buffer.

	Only a handful of scripts make a distinction between upper- and lowercase.
	In addition to modern scripts, such as Latin, Greek, Armenian and Cyrillic,
	a few historic or archaic scripts have case. The vast majority of scripts
	do not have case distinctions.

	\note Case mapping is not reversible. That is, `toUpper(toLower(x))
	!= toLower(toUpper(x))`.

	\note This function checks the (thread-local) system locale in order to
	support languages with exceptional behavior on specific code points.
	Unfortunately, no cross-platform way of setting and retrieving the system
	locale is available without adding dependencies to the library. Please
	refer to your operating system's manual to see how to setup the system
	locale on your target system.

	\par For more information on these exceptional code points, please refer
	to the text file made available by the Unicode Consortium:
	ftp://ftp.unicode.org/Public/UNIDATA/SpecialCasing.txt

	Example:

	\code{.c}
		void Book_SetTitle(book_t* book, const char* title)
		{
			size_t converted_size;
			int32_t errors;
			size_t i;

			converted_size = utf8totitle(title, strlen(title), book->title, sizeof(book->title) - 1, &errors);
			if (converted_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				memset(book->title, 0, sizeof(book->title));

				return;
			}
			book->title[converted_size] = 0;
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8tolower
	\sa utf8toupper
	\sa utf8casefold
*/
UTF8_API size_t utf8totitle(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Remove case distinction from UTF-8 encoded text.

	Case folding is the process of eliminating differences between code points
	concerning case mapping. It is most commonly used for comparing strings in a
	case-insensitive manner. Conversion is fully compliant with the Unicode 7.0
	standard.

	Although similar to lowercasing text, there are significant differences.
	For one, case folding does *not* take locale into account when converting.
	In some cases, case folding can be up to 20% faster than lowercasing the
	same text, but the result cannot be treated as correct lowercased text.

	Only two locale-specific exception are made when case folding text.
	In Turkish, U+0049 LATIN CAPITAL LETTER I maps to U+0131 LATIN SMALL LETTER
	DOTLESS I and U+0130 LATIN CAPITAL LETTER I WITH DOT ABOVE maps to U+0069
	LATIN SMALL LETTER I.

	Although most code points can be case folded in-place, there are notable
	exceptions. For example, U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE) maps
	to "U+0069 U+0307" (LATIN SMALL LETTER I and COMBINING DOT ABOVE) when
	converted to lowercase. Therefor, it is advised to first determine the size
	in bytes of the output by calling the function with a NULL output buffer.

	Only a handful of scripts make a distinction between upper- and lowercase.
	In addition to modern scripts, such as Latin, Greek, Armenian and Cyrillic,
	a few historic or archaic scripts have case. The vast majority of scripts
	do not have case distinctions.

	\note This function checks the (thread-local) system locale in order to
	support languages with exceptional behavior on specific code points.
	Unfortunately, no cross-platform way of setting and retrieving the system
	locale is available without adding dependencies to the library. Please
	refer to your operating system's manual to see how to setup the system
	locale on your target system.

	Example:

	\code{.c}
		int32_t Command_ParseCommand(const char* argument)
		{
			char* buffer = NULL;
			size_t buffer_size = 0;
			int32_t errors;
			int32_t result = 0;

			buffer_size = utf8casefold(argument, strlen(argument), NULL, 0, &errors);
			if (buffer_size == 0 ||
				errors != UTF8_ERR_NONE)
			{
				result = -1;

				goto cleanup;
			}

			buffer = (char*)malloc(buffer_size);

			utf8casefold(argument, strlen(argument), buffer, buffer_size, &errors);
			if (errors != UTF8_ERR_NONE)
			{
				result = -1;

				goto cleanup;
			}

			if (!strncmp(buffer, "-username", strlen("-username")))
			{
				result = eCommand_Username;
			}
			else if (
				!strncmp(buffer, "-password", strlen("-password")))
			{
				result = eCommand_Password;
			}
			else if (
				!strncmp(buffer, "-message", strlen("-message")))
			{
				result = eCommand_Message;
			}

		cleanup:
			if (buffer != NULL)
			{
				free(buffer);
				buffer = NULL;
			}

			return result;
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8tolower
	\sa utf8toupper
	\sa utf8totitle
*/
UTF8_API size_t utf8casefold(const char* input, size_t inputSize, char* target, size_t targetSize, int32_t* errors);

/*!
	\brief Check if a string is stable in the specified Unicode Normalization
	Form.

	This function can be used as a preprocessing step, before attempting to
	normalize a string. Normalization is a very expensive process, it is often
	cheaper to first determine if the string is unstable in the requested
	normalization form.

	The result of the check will be YES if the string is stable and MAYBE or NO
	if it is unstable. If the result is MAYBE, the string does not necessarily
	have to be normalized.

	If the result is unstable, the offset parameter is set to the offset for the
	first unstable code point. If the string is stable, the offset is equivalent
	to the length of the string in bytes.

	You must specify the desired Unicode Normalization Form by using a
	combination of flags:

	Unicode                      | Flags
	---------------------------- | ---------------------------------------------------------
	Normalization Form C (NFC)   | #UTF8_NORMALIZE_COMPOSE
	Normalization Form KC (NFKC) | #UTF8_NORMALIZE_COMPOSE + #UTF8_NORMALIZE_COMPATIBILITY
	Normalization Form D (NFD)   | #UTF8_NORMALIZE_DECOMPOSE
	Normalization Form KD (NFKD) | #UTF8_NORMALIZE_DECOMPOSE + #UTF8_NORMALIZE_COMPATIBILITY

	For more information, please review [Unicode Standard Annex #15 - Unicode
	Normalization Forms](http://www.unicode.org/reports/tr15/).

	Example:

	\code{.c}
		uint8_t Text_InspectComposed(const char* text)
		{
			const char* src = text;
			size_t src_size = strlen(text);
			size_t offset;
			size_t total_offset;

			if (utf8isnormalized(src, src_size, UTF8_NORMALIZE_COMPOSE, &offset) == UTF8_NORMALIZATION_RESULT_YES)
			{
				printf("Clean!\n");

				return 1;
			}

			total_offset = offset;

			do
			{
				const char* next;

				printf("Unstable at byte %d\n", total_offset);

				next = utf8seek(src, text, 1, SEEK_CUR);
				if (next == src)
				{
					break;
				}

				total_offset += offset;

				src = next;
				src_size -= next - src;
			}
			while (utf8isnormalized(src, src_size, UTF8_NORMALIZE_COMPOSE, &offset) != UTF8_NORMALIZATION_RESULT_YES);

			return 0;
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[in]   flags       Desired normalization form. Must be a combination of #UTF8_NORMALIZE_COMPOSE, #UTF8_NORMALIZE_DECOMPOSE and #UTF8_NORMALIZE_COMPATIBILITY.
	\param[out]  offset      Offset to first unstable code point or length of input in bytes if stable.

	\retval #UTF8_NORMALIZATION_RESULT_YES    Input is stable and does not have to be normalized.
	\retval #UTF8_NORMALIZATION_RESULT_MAYBE  Input is unstable, but normalization may be skipped.
	\retval #UTF8_NORMALIZATION_RESULT_NO     Input is unstable and must be normalized.

	\sa utf8normalize
*/
UTF8_API uint8_t utf8isnormalized(const char* input, size_t inputSize, size_t flags, size_t* offset);

/*!
	\brief Normalize a string to the specified Unicode Normalization Form.

	The Unicode standard defines two standards for equivalence between
	characters: canonical and compatibility equivalence. Canonically equivalent
	characters and sequence represent the same abstract character and must be
	rendered with the same appearance and behavior. Compatibility equivalent
	characters have a weaker equivalence and may be rendered differently.

	Unicode Normalization Forms are formally defined standards that can be used
	to test whether any two strings of characters are equivalent to each other.
	This equivalence may be canonical or compatibility.

	The algorithm puts all combining marks into a specified order and uses the
	rules for decomposition and composition to transform the string into one of
	four Unicode Normalization Forms. A binary comparison can then be used to
	determine equivalence.

	These are the Unicode Normalization Forms:

	Form                         | Description
	---------------------------- | ---------------------------------------------
	Normalization Form D (NFD)   | Canonical decomposition
	Normalization Form C (NFC)   | Canonical decomposition, followed by canonical composition
	Normalization Form KD (NFKD) | Compatibility decomposition
	Normalization Form KC (NFKC) | Compatibility decomposition, followed by canonical composition

	`utf8normalize` can be used to transform text into one of these forms. You
	must specify the desired Unicode Normalization Form by using a combination
	of flags:

	Form                          | Flags
	----------------------------- | ---------------------------------------------------------
	Normalization Form D (NFD)    | #UTF8_NORMALIZE_DECOMPOSE
	Normalization Form C (NFC)    | #UTF8_NORMALIZE_COMPOSE
	Normalization Form KD (NFKD)  | #UTF8_NORMALIZE_DECOMPOSE + #UTF8_NORMALIZE_COMPATIBILITY
	Normalization Form KC (NFKC)  | #UTF8_NORMALIZE_COMPOSE + #UTF8_NORMALIZE_COMPATIBILITY

	For more information, please review [Unicode Standard Annex #15 - Unicode
	Normalization Forms](http://www.unicode.org/reports/tr15/).

	\note Unnormalized text is rare in the wild. As an example, *all* text
	found on the Internet as HTML source code must be encoded as NFC, as
	specified by the W3C.

	Example:

	\code{.c}
		void Font_RenderTextNormalized(const char* input)
		{
			const char* src = NULL;
			const char* src_start;
			size_t src_size;
			char* converted = NULL;
			size_t converted_size = 0;
			size_t input_size = strlen(input);

			if (utf8isnormalized(input, input_size, UTF8_NORMALIZE_COMPOSE, NULL) != UTF8_NORMALIZATION_RESULT_YES)
			{
				int32_t errors;

				converted_size = utf8normalize(input, input_size, NULL, 0, UTF8_NORMALIZE_COMPOSE, &errors);
				if (converted_size > 0 &&
					errors == UTF8_ERR_NONE)
				{
					converted = (char*)malloc(converted_size + 1);
					utf8normalize(input, input_size, converted, converted_size, UTF8_NORMALIZE_COMPOSE, NULL);
					converted[converted_size] = 0;

					src = (const char*)converted;
					src_size = converted_size;
				}
			}

			if (src == NULL)
			{
				src = (const char*)input;
				src_size = input_size;
			}

			src_start = src;

			while (src_size > 0)
			{
				const char* next;
				int32_t errors;

				next = utf8seek(src, src_size, src_start, 1, SEEK_CUR);
				if (next == src)
				{
					break;
				}

				unicode_t code_point;
				utf8toutf32(src, (size_t)(next - src), &code_point, sizeof(unicode_t), &errors);
				if (errors != UTF8_ERR_NONE)
				{
					break;
				}

				Font_RenderCodePoint(code_point);

				src_size -= next - src;
				src = next;
			}

			if (converted != NULL)
			{
				free(converted);
				converted = NULL;
			}
		}
	\endcode

	\param[in]   input       UTF-8 encoded string.
	\param[in]   inputSize   Size of the input in bytes.
	\param[out]  target      Output buffer for the result, can be NULL.
	\param[in]   targetSize  Size of the output buffer in bytes.
	\param[in]   flags       Desired normalization form. Must be a combination of #UTF8_NORMALIZE_COMPOSE, #UTF8_NORMALIZE_DECOMPOSE and #UTF8_NORMALIZE_COMPATIBILITY.
	\param[out]  errors      Output for errors.

	\return Amount of bytes needed for storing output.

	\retval #UTF8_ERR_NONE                    No errors.
	\retval #UTF8_ERR_INVALID_FLAG            Invalid combination of flags was specified.
	\retval #UTF8_ERR_INVALID_DATA            Failed to decode data.
	\retval #UTF8_ERR_OVERLAPPING_PARAMETERS  Input and output buffers overlap in memory.
	\retval #UTF8_ERR_NOT_ENOUGH_SPACE        Target buffer size is insufficient for result.

	\sa utf8isnormalized
*/
UTF8_API size_t utf8normalize(const char* input, size_t inputSize, char* target, size_t targetSize, size_t flags, int32_t* errors);

UTF8_API size_t utf8iscategory(const char* input, size_t inputSize, size_t flags);

#endif /* _UTF8REWIND_H_ */