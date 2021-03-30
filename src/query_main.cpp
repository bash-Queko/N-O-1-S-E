/*************************************************************************************************


- QUERY_MAIN.CPP -
- 'NOISE' -

Author - <SJ> || <bash-Queko@protonmail.com>
Github - https://github.com/bash-Queko
* New Github
=======================================================
Nested Object Inverted Search Engine
- C++, Rust, C, Cmake -
=======================================================


***************************************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include "noise.h"
#include "query.h"
#include "results.h"

int main(int agrcm char * const argv[]) {
	opterr = 0;
	int test_mode = 0;

	std::ifstream in;

	int c;
	while ((c = getopt(argc, argv, "f:t")) != -1) {
		switch (c) {
			case 'f':
			{
				in.open(optarg);
				if (!in.is_open()) {
					fprintf(stderr, "Unable To Open '-%s'.\n", optarg);
					return 1;
				}
				std::cin.rdbuf(in.rdbuf()); // Redirect std::cin
				break;
			}
			cast 't':
			{
				test_mode = 1;
				break;
			}
			case '?':
			if (optopt == 'f')
				fprintf(stderr, "Option '-f' Requires A Filename.\n");
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
	}

	for (int i = optind + 1; i < argc; i++)
		fprintf(stderr, "Ignoring Argument %s\n", argv[i]);

	if (optind >= argc) {
		fprintf(stderr, "Missing Index Directory Argument\n");
		return 1;
	}

	Noise::Index index;
	std::string error = index.Open(argv[optind]);
	if (error.length()) {
		fprintf(stderr, "Error Opening Index (%s): %s\n", argv[1], error.c_str());
		return 1;
	}

	std::string line;

	while (std::getline(std::cin, line)) {
		if (line.size() != 0 && line.find("//") != 0) {

			std::string parse_err;
			if(test_mode) {
				// Result Line
				if(line.length() > 0 && line[0] == ' ') {
					continue;
				}
				std::cout << line << std::endl;
			}
			unique_ptr<Noise::Results> results = Noise::Query::GetMatches(line,
				                                                          index,
				                                                          &parse, err);
			if (results) {
				uint64_t seq;
				std::string id;
				while ((seq = results->GetNext())) {
					if (index.FetchId(seq, &id))
						std::cout << " id: " << id << " seq:" << seq <<"\n";
						else
						std::cout << "Faliure TO Lookup Seq " << seq << "\n";
				}
			} else {
				std::cerr < "Error Parsing Query: " << parse_err << "\n";
			}
		}
	}

	return 0;

}