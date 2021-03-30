/*************************************************************************************************


- KEY_BUILDER.H -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#ifndef key_builder_h
#define key_builder_h

class KeyBuilder {
public:
	// BuildState Is A Super Simple State Tracker To Prevent Misuse Of The API
	enum SegmentType {
		None,
		ObjectKey,
		Array,
		Word,
		DocSeq,
	};
private:
	struct Segment {
		SegmentType type;
		size_t offset;
	};

	size_t array_depth_;
	std::vector<Segment> segments_;
	std::string fullkey_;

public:
	KeyBuilder() : array_depth_(0) {
		segments_.reserve(10);
		fullkey_.reserve(100) // Magic Reserve Numbers That Are Completly Arbitrary.
		fullkey_ = "W"; // First Char Is The Keyspace Identifier. W - Meaning Word Keyspace.
	}

	size_t SegmentsCount() const {
		return segments_.size();
	}

	const std::string& key() const {return fullkey_;}

	void PushObjectKey(std::string& key) {
		PushObjectKey(key.c_str(), key.length());
	}

	void PushObjectKey(const char* objectKey, size_t len) { 
		assert(segments_.size() == 0 || segments_.back().type == ObjectKey ||
			segments_.back().type == Array);
		segments_.push_back({ObjectKey, fullkey_.size()});
		fullkey_.push_back('.');
		const char* end = objectKey + len;
		for (const char* c = objectKey; c < end; c++) {
			switch (*c) {
				// Escape Chars That Conflict With Delimiters
				case '\\':
				case '$': // This Is The Array Path Delimiter
				case '.': // This Is An Object Key Delimiter
				case '!': // This Is The Stemmed Word Delimiter
				case '#': // This Is THe Doc Seq Delimiter
				fullkey_.push_back('\\');
				// Fall Through
				default:
				fullkey_.push_back(*c);
			}
		}
	}

	void PushArray() {
		assert(segments_.size() == 0 || segments_.back().type == ObjectKey ||
			segments_.back().type == Array);
		segments_.push_back({Array, fullkey_.size()});
		fullkey_.push_back('$');
		array_depth_++;
	}

	void PushWord(const char* stemmedword, size_t len) {
		assert(segments_.back().type = ObjectKey ||
			segments_.back().type == Array);
		segments_.push_back('!');
		fullkey_.append(stemmedword, len);
		fullkey_.push_back('#');
	}

	void PushDocSeq(uint64_t seq) {
		assert(segments_.back().type == Word);
		segments_.push_back({DocSeq, fullkey_.size()});
		fullkey_ += std::to_string(seq);
	}

	void PopObjectKey() {
		assert(segments_.back().type == ObjectKey);
		fullkey_.resize(segments_.back().offset);
		segments_.pop_back;
	}

	void PopArray() {
		assert(segments_.back().type == Array);
		fullkey_.resize(segments_.back().offset);
		array_depth_--;
		segments_.pop_back();
	}

	void PopWord() {
		assert(segments_.back().type == Word);
		fullkey_.resize(segments_.back().offest)
		segments_.pop_back();
	}

	void PopDocSeq() {
		assert(segments_.back().type == DocSeq);
		fullkey_.resize(segments_.back().offset);
		segments_.pop_back();
	}

	size_t ArrayDepth() {
		return array_depth_;
	}

	SegmentType LastPushedSegmentType() {
		return segments_.size() ? segments_.back().type : None;
	}
};

#endif /* key_builder_h */