/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "coordinate.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace {

    /**
     * Really "does this look like one half of a WGS 84 DD
     * coordinate"?
     */
    bool is_float(const std::string& s)
    {
	if (std::count(begin(s), end(s), '.') != 1) return false;

	const auto p = s.c_str();
	char* end;
	std::strtod(p, &end);
	return end != p && !*end;
    }

    bool is_float(const Record::spair& coord)
    {
	return is_float(coord[0]) && is_float(coord[1]);
    }

    bool size(const Record::spair& coord, unsigned a, unsigned b)
    {
	return coord[0].size()==a && coord[1].size()==b;
    }

    constexpr char nbsp[] = R"(\~)";

    void generic(std::ostream& os, const Record::spair& coord)
    {
	os << coord[0] << nbsp << coord[1];
    }

    void wgs84(std::ostream& os, const Record::spair& coord)
    {
	generic(os, coord);
	os << nbsp << R"(\s-2[WGS\|84]\s0)";
    }

    void rt90(std::ostream& os, const Record::spair& ne)
    {
	/* I really like the traditional convention of typesetting the
	 * two first digits of an RT90 coordinate (the 100 km squares)
	 * smaller.
	 *
	 * People could do the same with SWEREF99, but I suspect in
	 * this barbaric age of bad typography, noone has cared enough
	 * to form such a convention, and I'm not going to try to be
	 * that person.
	 */
	constexpr char sm[] = R"(\s-2)";
	constexpr char s0[] = R"(\s0)";

	const auto na = ne[0].substr(0, 2);
	const auto nb = ne[0].substr(2);
	const auto ea = ne[1].substr(0, 2);
	const auto eb = ne[1].substr(2);

	os << sm << na << s0 << nb
	   << nbsp
	   << sm << ea << s0 << eb
	   << nbsp << sm << "[RT90]" << s0;
    }

    void sweref99(std::ostream& os, const Record::spair& ne)
    {
	generic(os, ne);
	os << nbsp << R"(\s-2[SWEREF99]\s0)";
    }
}

/**
 * Print different ways, depending on what coordinate system we manage
 * to guess:
 *
 *   RT90 6395753 1274177 (±10m)       RT90 2.5 gon
 *   SWEREF99 6391427 322318 (±10m)    SWEREF99 TM
 *   WGS84 57.63009 12.02405 (±10m)    WGS 84 DD
 *   xxxx yyyy (±10m)                  unknown
 *
 * We guess because there's no indication in the input what coordinate
 * system is in use; it's a per-user setting in Artportalen.
 */
std::ostream& Coordinate::put(std::ostream& os) const
{
    if (is_float(ne)) {
	wgs84(os, ne);
    }
    else if (size(ne, 7, 7)) {
	rt90(os, ne);
    }
    else if (size(ne, 7, 6)) {
	sweref99(os, ne);
    }
    else {
	generic(os, ne);
    }

    if (radius.empty()) return os;

    os << R"(\~(\[t+-])" << radius << R"(\|m))";
    return os;
}
