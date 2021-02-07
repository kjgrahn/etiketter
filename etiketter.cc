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

#include "key.h"

namespace {

    const char* version()
    {
	return "1.0";
    }

    constexpr char nl = '\n';

    void preamble(std::ostream& os)
    {
	os << "\\\" -*- nroff -*-" << nl
	   << ".ig" << nl
	   << "This is troff/groff markup, generated from data" << nl
	   << "presumably from Artportalen, by etiketter " << version() << "." << nl
	   << "See the groff(7) man page if you need to understand" << nl
	   << "the markup." << nl
	   << ".." << nl
	   << ".ll 10c" << nl
	   << ".pl 7c" << nl
	   << "." << nl;
    }

    struct text {
	text (const std::string& val, const std::string& b = "")
	    : val(val),
	      b(b)
	{}
	const std::string& val;
	const std::string& b;
    };

    std::ostream& operator<< (std::ostream& os, const text& val)
    {
	return os << val.val << val.b << nl;
    }

    const char* coord(Record::spair, std::string) { return "XXX\n"; }

    bool etikett(std::ostream& os, const Record& e)
    {
	os << ".ps 20" << nl
	   << text("Flora Suecica")
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
	   << text(e.id())
	   << ".br" << nl
	   << text(e.name())
	   << ".sp" << nl
	   << "." << nl
	   << text(e.province(), ",")
	   << text(e.parish())
	   << ".br" << nl
	   << text(e.place())
	   << ".sp 6p" << nl
	   << coord(e.coordinate(), e.radius())
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
}

int main()
{
    Key key;

    std::string s;
    while (std::getline(std::cin, s)) {
	if (key.feed(s)) break;
    }

    if (!key.valid()) {
	std::cerr << "error: found no key/heading row in the data\n";
	return 1;
    }

    preamble(std::cout);

    while (std::getline(std::cin, s)) {
	const Record record {key, s};
	if (!etikett(std::cout, record)) return 1;
    }

    return 0;
}
