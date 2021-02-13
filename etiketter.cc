/*
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 or version 3 of the
 *    License.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * (Actually, you received no such copy.)
 *
 */
#include <string>
#include <iostream>

#include <getopt.h>

#include "key.h"
#include "thousands.h"
#include "coordinate.h"

namespace {

    const char* version()
    {
	return "1.0";
    }

    constexpr char nl = '\n';

    void preamble(std::ostream& os)
    {
	os << R"(.\" -*- nroff -*-)" << nl
	   << ".ig" << nl
	   << "This is troff/groff markup, generated from data" << nl
	   << "presumably from Artportalen, by etiketter " << version() << "." << nl
	   << "See the groff(7) man page if you need to understand" << nl
	   << "the markup." << nl
	   << ".." << nl
	   << ".de TITLE" << nl
	   << ".sp" << nl
	   << ".ps 20" << nl
	   << R"(F\s-4LORA\s0 S\s-4UECICA\s0)" << nl
	   << ".." << nl
	   << "." << nl
	   << ".ll 10c" << nl
	   << ".pl 7c" << nl
	   << "." << nl;
    }

    template <class T>
    struct Text {
	Text (const T& val, const std::string& b = "")
	    : val(val),
	      b(b)
	{}
	const T& val;
	const std::string& b;
    };

    template <class T>
    std::ostream& operator<< (std::ostream& os, const Text<T>& val)
    {
	return os << val.val << val.b << nl;
    }

    const char* coord(Record::spair, std::string) { return "XXX\n"; }

    bool etikett(std::ostream& os, const Record& e)
    {
	using text = Text<std::string>;

	os << ".TITLE" << nl
	   << ".ps 14" << nl
	   << ".sp 2" << nl
	   << "." << nl
	   << ".mk" << nl
	   << ".ft BI" << nl
	   << text(e.taxon())
	   << ".ft" << nl
	   << ".ps 12" << nl
	   << text(e.auctor())
	   << ".sp" << nl
	   << ".ps 10" << nl
	   << ".rj 1" << nl
	   << ".rt" << nl
	   << Text<Thousands>(e.id())
	   << ".br" << nl
	   << text(e.name())
	   << ".sp" << nl
	   << "." << nl
	   << text(e.province(), ",")
	   << text(e.parish())
	   << ".br" << nl
	   << text(e.place())
	   << ".sp 6p" << nl
	   << Coordinate(e.coordinate(), e.radius()) << nl
	   << ".sp" << nl
	   << text(e.comment())
	   << ".sp" << nl
	   << ".mk" << nl
	   << text(e.date())
	   << ".rj 1" << nl
	   << ".rt" << nl
	   << text("Leg \\fI" + e.leg() + "\\fP");

	os << "." << nl
	   << ".bp" << nl;

	return true;
    }

    int etiketter(std::ostream& os, std::istream& is)
    {
	Key key;

	std::string s;
	while (std::getline(is, s)) {
	    if (key.feed(s)) break;
	}

	if (!key.valid()) {
	    std::cerr << "error: found no key/heading row in the data\n";
	    return 1;
	}

	preamble(os);

	while (std::getline(is, s)) {
	    const Record record {key, s};
	    if (!etikett(os, record)) return 1;
	}

	return 0;
    }
}

int main(int argc, char ** argv)
{
    const std::string prog = argv[0] ? argv[0] : "etiketter";
    const std::string usage = std::string("usage: ")
	+ prog + " file ...\n"
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
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "etiketter " << version() << '\n'
		      << "Copyright (c) 2021 Jörgen Grahn.\n"
		      << "All rights reserved.\n";
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

    const std::vector<std::string> files(argv+optind, argv+argc);
    if (files.size()) {
	// XXX handle not reading from stdin
    }

    return etiketter(std::cout, std::cin);
}
