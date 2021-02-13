/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ETIKETT_THOUSANDS_H
#define ETIKETT_THOUSANDS_H

#include <string>
#include <iosfwd>

/**
 * For dividing a string in groups of three like "1,000" when
 * streaming it.
 */
class Thousands {
public:
    Thousands(const std::string& s,
	      const std::string& sep = "\\|");
    std::ostream& put(std::ostream& os) const;

private:
    const std::string s;
    const std::string sep;
};

inline
std::ostream& operator<< (std::ostream& os, const Thousands& val) { return val.put(os); }

#endif
