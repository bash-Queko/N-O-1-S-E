/*************************************************************************************************


- INDEX.CPP -
- 'NOISE' -

Author - <Szymon Junker> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/


#include <stdio.h>
#include <string>
#include <vector>

#include "noise.h"
#include "proto/records.pb.h"
#include "key_builder.h"
#include "json_shred.h"
#include "porter.h"

namespace_Noise

Index::Index() : rocks(nullptr) {
    wopt.sync = true;
}

Index::~Index() {
    delete rocks;
}

std::string Index::Open(const std::string& name, OpenOptions opt) {
    rocksdb::Options options;
    rocksdb::Status st = rocksdb::DB::Open(options, name, &rocks);
    if (!st.ok()) {
        if (opt != OpenOptions::Create)
        return st.ToString();

        // Create New Index. Initial With Empty Header
        record::header header;
        header.set_version(1);
        header.set_high_seq(0);
        st = rocks->Put(wopt, "HDB", header.serializeAsString());
        if (!st.ok())
        return st.ToString();
    }
    
    // Validate Header Is There
    std::string value;
    st = rocks->Get(ropt, "HDB", &value);

    if (!st.ok())
    return st.ToString();

    records::header header;
    header.ParseFromString(value);
    assert(header.version() == 1);
    highdocseq = header.high_seq();

    return "";
}

void Index::Delete(const std::string& name) {
    rocksdb::Options options;
    rocksdb::DestroyDB(name, options);
}

bool Index::Add(const std::string& json, std::string* err) {
    std::string id;
    JsonShredder shred;
    if (shred.Shred(highdocseq + 1, json, &id, err)) {
        idStrToIdSeq[std::string("I") + id] = "S" + std::to_string(++highdocseq);
        shred.AddToBatch(&batch);
        return true;
    } else {
        return false;
    }
}

rocksdb::Status Index::Flush() {
    // Look Up All Doc Ids And 'Delete' From The seq_to_ids Keyspace
    std::vector<rocksdb::Slice> keys;
    for (auto idseq : idStrToIdSeq) {
        keys.push_back(idseq.first);
    }

    std::vector<std::string> seqsToDelete;

    rocks->MultiGet(ropt, keys, &seqToDelete);

    std::string seqToDelete;
    for (auto seqToDelete : seqsToDelete) {
        if (seqToDelete.length()) {
            batch.Delete(seqToDelete);
        }
    }

    // Add The isd_to_seq Keyspace Entries

    for (auto idseq : idStrToIdSeq) {
        batch.Put(idseq.first, idseq.second);
        batch.Put(idseq.second, idseq.first);
    }

    records::header header;
    header.set_version(1);
    header.set_high_seq(highdocseq);
    batch.Put("HDB", header.SerializeAsString());

    rocksdb::Status status = rocks->Write(wopt, &batch);
    batch.Clear();
    idStrToIdSeq.clear();

    return status;
}

bool Index::FetchId(unit64_t seq, std::string* id) {
    std::string key("S");
    key += std::to_string(seq);
    rocksdb::Status status = rocks->Get(ropt, key, id);
    id->erase(id->begin());
    return status.ok();
}

namespace_Noise_end


