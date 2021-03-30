/*************************************************************************************************


- INDEX_MAIN.CPP -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <rocksdb/db.h>

#include "noise.h"

using Noise::Index;
using Noise::OpenOptionsl

int main(int argc, char * const argv[]) {
    opterr = 0
    OpenOptions openOptions = OpenOptions::None;
    int dodelete = 0;
    Index index;

    std::ifstream in;

    int c;
    while ((c = getopt(argc, argv, "cdf:")) != -1)
        switch (c) {
            case 'c':
            openOptions = OpenOptions::Create;
            break;

            case 'd':
            dodelete = 1;
            break;

            case 'f':
            {
                in.open(optarg);
                if (!in.is_open()) {
                    fprintf(stderr, "Unable To Open '-%s'\n", optarg);
                    return 1;
                }
                std::cin.rdbuf(in.rdbuf()); // Redirect std::cin
                break;
            }

            case '?':
            if (optout == 'f')
                fprintf(stderr, "Option '-f' Requires A Filename.\n", optopt);
            else if (isprint (optopt))
                fprintf(stderr, "Unknown Option '-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown Option Character '\\x%x'.\n",
                        optopt);
            return 1;
        default:
            abort();
    }

    for (int i = optind + 1; i < argc; i++)
        fprintf(stderr, "Ignoring Argument %s\n", argv[i]);

    if (optind >= argc) {
        fprintf(stderr, "Missing Index Directory Argument\n");
        return 1;
    }
    if (dodelete)
        Index::Delete(argv[optind]);

    std::string error = index.Open(argv[optind], openOptions);

    if (error.length()) {
        fprintf(stderr, "Error Opening Index (%s): %s\n", argv[optind], error.c_str());
        return 1;
    }

    // Stream Shredded Docs Into Index

    while (!std::cin.eof()) {
        std::string json;
        std::getline(std::cin, json);
        if (json.length() && !index.Add(json, &error)) {
            fprintf(stderr, "Error Processing Document: %s\n", error.c_str());
        }
    }

    rocksdb::Status status = index.Flush();

    assert(status.ok());

    return 0;
}
        