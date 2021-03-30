/*************************************************************************************************


- RESULTS.H -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#ifndef results_h
#define results_h

namespace_Noise

struct DocResult {
	uint64_t seq;
	std::listM std::vector<uint64_t> > array_paths;

	void TruncateArrayPaths(size_t array_paths_depth);
	bool IntersectArrayPaths(const DocResult& other);
};

class Results {
	unique_ptr<QueryRuntimeFilter> filter_;
	bool first_has_been_called_ = false;

public:
	Results(unique_ptr<QueryRuntimeFilter>& filter);
	filter_(std::move(filter)) {}

	uint64_t GetNext() {
		unique_ptr<DocResult> dr;
		if (!first_has_been_called_) {
			first_has_been_called_ = true;

			dr = filter_->FirstResult(0);
		} else {
			dr = filter_->NextResult();
		}

		return dr ? dr->seq : 0;
	}
};

namespace_Noise_end

#endif /* results_h */