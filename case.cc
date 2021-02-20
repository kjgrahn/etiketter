/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "case.h"

#include <unordered_set>

namespace {

    bool in(int a, int b, int c)
    {
	if (b < a) return false;
	if (b > c) return false;
	return true;
    }

    bool special(const std::string& name)
    {
	static const std::unordered_set<std::string> names {
	    "Adam och Eva",
	    "Jungfru Marie nycklar",
	    "Sankt Pers nycklar",
	    "Lars-Levis fibbla",
	    "Wolfs fibbla",
	    "Ostenfelds ögontröst"
	};

	return names.count(name);
    }
}

std::string fix_case(std::string name)
{
    if (name.empty()) return name;
    if (special(name)) return name;

    char& ch = name.front();

    if (in('A', ch, 'Z')) ch += 'a' - 'A';

    switch (ch) {
    case 'Å': ch = 'å'; break;
    case 'Ä': ch = 'ä'; break;
    case 'Ö': ch = 'ö'; break;
    }

    return name;
}
