/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef XLSX_STRINGS
#define XLSX_STRINGS

#include <string>
#include <vector>

namespace xlsx {

    class SharedStrings {
    public:
	explicit SharedStrings(const std::string& data);

	const std::string& operator[] (unsigned n) const;

    private:
	std::vector<std::string> val;
    };
}

#endif
