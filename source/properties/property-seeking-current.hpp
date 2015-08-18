#pragma once

#include "property-base.hpp"
#include "property-generators.hpp"

class PropertySeekingCurrent
	: public quickcheck::Property<std::string, off_t>
{

	bool holdsFor(const std::string& input, const off_t& offset)
	{
		const char* seek_next = utf8seek(
			input.c_str(), input.c_str(),
			offset, SEEK_CUR);

		const char* seek_start = utf8seek(
			seek_next, input.c_str(),
			-offset, SEEK_CUR);

		return seek_start == input.c_str();
	}

	const std::string classify(const std::string& text, const off_t& offset)
	{
		std::stringstream ss;
		ss << "in sequence of length " << utf8len(text.c_str()) << " offset by " << offset;
		return ss.str();
	}

	void generateInput(size_t sizeHint, std::string& text, off_t& offset)
	{
		size_t generated_length;

		if (sizeHint < 10)
		{
			generated_length = 1;
		}
		else if (sizeHint < 80)
		{
			generated_length = quickcheck::generateInRange(2, 15);
		}
		else
		{
			generated_length = quickcheck::generateInRange(16, 100);
		}

		for (size_t i = 0; i < generated_length; ++i)
		{
			std::string grapheme;
			quickcheck::generate(quickcheck::generateInRange(0, 100), grapheme);

			text += grapheme;
		}

		offset = (ptrdiff_t)generated_length;
	}

};