/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "thousands.h"

#include <iostream>

Thousands::Thousands(const std::string& s,
		     const std::string& sep)
    : s {s},
      sep {sep}
{}

std::ostream& Thousands::put(std::ostream& os) const
{
    // pn 3 * n
    const unsigned pn = s.size() % 3;
    const unsigned n = s.size() / 3;

    const char* p = s.c_str();

    os.write(p, pn);
    p += pn;
    for (unsigned i=0; i<n; i++) {
	if (i || pn) os << sep;
	os.write(p, 3);
	p += 3;
    }

    return os;
}
