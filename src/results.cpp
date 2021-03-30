/*************************************************************************************************


- RESULTS.CPP -
- 'NOISE' -

Author - <Szymon Junker> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#include "noise.h"
#include "query.h"
#include "results.h"

namespace_Noise

void DocResult::TruncateArrayPaths(size_t array_path_depth) {
    for(auto& ap : array_paths)
    ap.resize(array_path_depth);
}
bool DocResult::IntersectArrayPaths(const DocResult& other) {
    for (auto ap = array_paths.begin(); ap != array_paths.end(); ap++) {
        bool found = false;
        for (auto& ap_other : other.array_paths) {
            if (*ap == ap_other) {
                found = true;
                break;
            }
        }
        if (!found) {
            auto del = ap++;
            array_paths.remove(*del);
        }
    }
    return array_paths.size() > 0;
}

namespace_Noise_end