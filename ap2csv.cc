/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "xlsx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#include <errno.h>
#include <getopt.h>

int main(int argc, char ** argv)
{
    const std::string prog = argv[0];
    const std::string usage = std::string("usage: ")
	+ prog + " [file]\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "";
    const struct option long_options[] = {
	{"help", 0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring,
			    &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "ap2csv 1.0\n"
		      << "Copyright (c) 2021 J Grahn\n";
	    return 0;
	    break;
	case ':':
	case '?':
	default:
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	}
    }

    const std::vector<std::string> args {argv+optind, argv+argc};
    if (args.size() > 1) {
	std::cerr << usage << '\n';
	return 1;
    }

    if (args.size() == 1) {
	const auto& filename = args[0];
	std::ifstream in {filename};
	if (!in) {
	    std::cerr << "error: cannot open " << filename
		      << " for reading: " << std::strerror(errno) << '\n';
	    return 1;
	}

	xlsx::csv(std::cout, in);
    }
    else {
        xlsx::csv(std::cout, std::cin);
    }

    return 0;
}
