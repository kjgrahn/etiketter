/* Copyright (c) 2021 J�rgen Grahn
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

    bool ends(const std::string& haystack, const std::string& needle)
    {
	if (haystack.size() < needle.size()) return false;
	return haystack.rfind(needle) == haystack.size() - needle.size();
    }

    bool special(const std::string& name)
    {
	static const std::unordered_set<std::string> names {
	    "Adam och Eva",
	    "Jungfru Marie nycklar",
	    "Sankt Pers nycklar",
	    "Lars-Levis fibbla",
	    "Wolfs fibbla",
	    "Ostenfelds �gontr�st",
	    "Birgittamaskros",
	};

	return names.count(name);
    }

    bool someones(const std::string& name)
    {
	return ends(name, "s bj�rnb�r") || ends(name, "s maskros");
    }
}

std::string fix_case(std::string name)
{
    if (name.empty()) return name;
    if (special(name)) return name;
    if (someones(name)) return name;

    char& ch = name.front();

    if (in('A', ch, 'Z')) ch += 'a' - 'A';

    switch (ch) {
    case '�': ch = '�'; break;
    case '�': ch = '�'; break;
    case '�': ch = '�'; break;
    }

    return name;
}
