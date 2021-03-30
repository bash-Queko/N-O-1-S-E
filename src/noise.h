/*************************************************************************************************


- NOISE.H -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#ifndef noise_h
#define noise_h

// This Is An Egrigeous Trick To Work Around XCodes Editor Identing Inside Parens
#define namespace_Noise namespace Noise {
#define namespace_Noise_end };

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <list>
#include <memory>

#include <rocksdb/db.h>

using std::unique_ptr;

namespace_Noise

enum OpenOptions {
    None,
    Create
};

class Index {
    rocksdb::DB* rocks;
    rocksdb::WriteOptions wopt;
    rocksdb::ReadOptions ropt;

    rocksdb::WriteBatch batch;

    uint64_t highdocseq;
    std::map<std::string,std::string> idStrToIdSeq;

public:
    Index();
    ~Index();

    rocksdb::DB* GetDB() {return rocks;}

    std::string Open(const std::string& name, OpenOptions opt=None);

    static void Delete(const std::string& name);

    bool Add(const std::string& json, std::string* err);

    bool FetchId(uint64_t seq, std::string* id);

    rocksdb::Status Flush();
};

namespace_Noise_end

#endif /* noise_h */