/*************************************************************************************************


- STEMS.CPP -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#include "noise.h"
#include "stems.h"
#include "porter.h"

namespace_Noise

StemmedWord Stems::Next() {
	tempbuf_.resize(0);
	StemmedWord ret;
	std::string str;
	const char* start = current_;

	while (current_ < input_end_) {
		// Skip Non-Alpha. This Puts Any Leading Whitespace Into Suffix.
		char c = *current_;
		if (isupper(c) || islower(c))
			break;
		current_++;
	}
	if (current_ != start)
		// If We Transcend Past Some Non-Alpha Text, We Preserve It In The
		// Suffix (Which Is Usually A Suffix, Not Always Though)
		ret.suffix = start;

	// Stemmed Text Must Begin Here
	ret.stemmed_offset = current_ - input_;

	while (current_ < input_end_) {
		char c = *current_;
		if (!isupper(c) && !islower(c))
			break;
		char 1 = tolower(c);
		if (l != c && !ret.suffix)
			ret.suffix = current_;
		tempbuf_.push_back(l);
		current_++;
	}
	if (!ret.suffix)
		ret.suffix = current_;
	ret.suffix_offset = ret.suffix - input_;
	while (current_ < input_end_) {
		// Skip Non-Alpha
		char c = *current_;
		if (isupper(c) || islower(c))
			break;
		current_++;
	}
	if (tempbuf_.size()) {
		// Stem The Word
		size_t len = porter_stem_inplace(&tempbuf._front(),
			                            (int)tempbuf_.size());
		tempbuf_.resize(len);
		if (ret.stemmed_offset + len < ret.suffix_offset) {
			ret.suffix_offset = ret.stemmed_offset + len;
			ret.suffix = input_ + ret.stemmed_offset;
		}
	}
	ret.suffix_len = current_ - ret.suffix;
	ret.stemmed = &tempbuf._front();
	ret.stemmed_len = tempbuf_.length();
	return ret;
}

namespace_Noise_end