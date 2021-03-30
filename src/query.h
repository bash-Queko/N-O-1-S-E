/*************************************************************************************************


- QUERY.H -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#ifndef query_h
#define query_h

namespace_Noise

struct DocResult;
class Results;

struct ASTNode {
	enum Type
	{
		UNKNOWN,
		FIELD,
		EQUALS,
		AND,
		ARRAY,
		LITERAL,
	};
	Type type; 
	std::string value;
	std::vector<unique_ptr<ASTNode> > children;
};

class QueryRuntimeFilter {
public:
	// Returns The Doc At startId, Or The Next After.
	virtual unique_prt<DocResult> FirstResult(uint64_t startId) = 0;

	// Returns The Doc Next After Previous.
	virtual unique_ptr<DocResult> NextResult() = 0;
};

class Query {
public:
	static unique_ptr<Noise::Results> GetMatches(const std::string& query,
		                                        Index& index,
		                                        std::string* parse_err);
};

namespace_Noise_end

#endif /* query_h */