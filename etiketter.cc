/*
 * Copyright (c) 2021 J�rgen Grahn
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
#include <fstream>
#include <cstring>
#include <memory>

#include <getopt.h>

#include "xlsx.h"
#include "key.h"
#include "thousands.h"
#include "coordinate.h"
#include "case.h"

namespace {

    const char* version()
    {
	return "2.2";
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
	   << ".na" << nl
	   << ".ss 12 0" << nl
	   << "." << nl;
    }

    void a7(std::ostream& os)
    {
	os << R"(.\" A7 105 � 74 mm)" << nl
	   << R"(.\" width minus 12 mm margins = 81 mm)" << nl
	   << ".po 1.2c" << nl
	   << ".ll 8.1c" << nl
	   << ".pl 7.4c" << nl
	   << "." << nl;
    }

    template <class T>
    struct Text {
	Text (const T& val,
	      const std::string& b = "",
	      const std::string& a = "")
	    : val(val),
	      a(a),
	      b(b)
	{}
	const T& val;
	const std::string& a;
	const std::string& b;
    };

    template <class T>
    std::ostream& operator<< (std::ostream& os, const Text<T>& val)
    {
	if (val.val.empty()) return os;
	return os << val.a << val.val << val.b << nl;
    }

    /**
     * Turn "[foo]" to "foo" and leave other strings unchanged.
     * Exists because Artportalen, for some reason, puts brackets
     * around names if the record has 'Ospontan' set.
     */
    std::string unbracket(const std::string& s)
    {
	if (s.size() < 2) return s;
	if (s.front()!='[' || s.back()!=']') return s;
	return {s.begin()+1, s.end()-1};
    }

    bool etikett(std::ostream& os, std::string user, const Record& e)
    {
	using text = Text<std::string>;
	constexpr char sm[] = R"(\s-4)";
	constexpr char s0[] = R"(\s0)";

	if (user.empty()) user = e.leg();

	/* This may be a stupid way of doing layouts in groff. Anyway,
	 * what I do is, most of the text is paragraphs, with font
	 * changes and various fixed spacing inbetween.
	 *
	 * Then for three pieces of information (id, date, leg): when
	 * I'm at a well-defined place on the paper I set a mark, jump
	 * a hardcoded amount, paint the text, and jump back.
	 *
	 * This is how date and leg form a footer, and how id shares a
	 * line with some shorter piece of information. It doesn't
	 * guarantee, however, that text doesn't run together. To
	 * guarantee that you have to have a less fixed overall
	 * layout, and I don't know how to create one of those.
	 */

	os << ".TITLE" << nl
	   << ".ps 10" << nl
	   << ".sp 1.6" << nl
	   << "." << nl
	   << ".mk" << nl
	   << "." << nl;

	os << ".sp 5c" << nl
	   << text(e.date())
	   << ".br" << nl
	   << ".rt" << nl
	   << "." << nl;

	os << ".sp 5c" << nl
	   << ".rj 1" << nl
	   << text(user, "\\fP", "Leg \\fI")
	   << ".rt" << nl
	   << "." << nl;

	os << ".ps 14" << nl
	   << ".ft BI" << nl
	   << text(unbracket(e.taxon()))
	   << ".ft" << nl
	   << text(unbracket(e.auctor()), s0, sm)
	   << ".br" << nl
	   << ".ps 10" << nl
	   << text(unbracket(fix_case(e.name())))
	   << ".sp" << nl
	   << ".mk" << nl
	   << "." << nl;

	if (!e.id().empty()) {

	    os << ".rj 2" << nl
	       << Text<Thousands>(e.id())
	       << ".sp -22p" << nl
	       << text("Artportalen", s0, sm)
	       << ".rt" << nl
	       << "." << nl;
	}

	os << text(e.province(), "\\fP,", "\\fB")
	   << text(e.parish())
	   << ".br" << nl
	   << text(e.place())
	   << ".br" << nl
	   << Coordinate(e.coordinate(), e.radius()) << nl
	   << ".sp 6p" << nl
	   << text(e.comment());

	return true;
    }

    /**
     * True if there's no point making a label out of this record.
     */
    bool useless(const Record& e, std::string user)
    {
	if (user.empty()) user = e.leg();
	if (user.empty()) return true;
	return e.name().empty() || e.taxon().empty();
    }

    int etiketter(std::ostream& os, std::istream& is,
		  const std::string& user)
    {
	const xlsx::Source source {is};
	auto it = source.begin();
	const auto end = source.end();
	Key key;

	while (it != end) {
	    if (key.feed(*it++)) break;
	}

	if (!key.valid()) {
	    std::cerr << "error: found no key/heading row in the data\n";
	    return 1;
	}

	preamble(os);
	a7(os);

	unsigned n = 0;

	while (it != end) {

	    const Record record {key, *it++};
	    if (useless(record, user)) continue;

	    if (n++) {
		os << "." << nl
		   << ".bp" << nl;
	    }

	    if (!etikett(os, user, record)) return 1;
	}

	return 0;
    }

    std::istream& or_stdin(std::ifstream& is)
    {
	if (is.is_open()) return is;
	return std::cin;
    }

    std::ostream& or_stdout(std::ofstream& os)
    {
	if (os.is_open()) return os;
	return std::cout;
    }
}

int main(int argc, char ** argv)
{
    const std::string prog = argv[0] ? argv[0] : "etiketter";
    const std::string usage = std::string("usage: ")
	+ prog + " [-u user] [-o file] [file]\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "u:o:";
    const struct option long_options[] = {
	{"help", 0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::string user;
    std::string outfile;

    int ch;
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'u':
	    user = optarg;
	    break;
	case 'o':
	    outfile = optarg;
	    break;
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "etiketter " << version() << '\n'
		      << "Copyright (c) 2021 J�rgen Grahn.\n"
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

    const std::vector<char*> files(argv+optind, argv+argc);
    if (files.size() > 1) {
	std::cerr << "error: too many arguments\n"
		  << usage << '\n';
	return 1;
    }

    std::ifstream is;
    if (files.size() == 1) {
	const auto filename = files[0];
	is.open(filename);
	if (!is) {
	    std::cerr << "error: cannot open " << filename << " for reading: "
		      << std::strerror(errno) << '\n';
	    return 1;
	}
    }

    std::ofstream os;
    if (outfile.size()) {
	os.open(outfile);
	if (!os) {
	    std::cerr << "error: cannot open " << outfile << " for writing: "
		      << std::strerror(errno) << '\n';
	    return 1;
	}
    }

    return etiketter(or_stdout(os), or_stdin(is),
		     user);
}
