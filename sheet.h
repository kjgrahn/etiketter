/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef XLSX_SHEET
#define XLSX_SHEET

#include "strings.h"

#include <string>

namespace xlsx {

    class Sheet {
    public:
	Sheet(const std::string& path, const SharedStrings& strings);
	auto begin() const { return val.begin(); }
	auto end() const { return val.end(); }

    private:
	std::vector<std::vector<std::string>> val;
    };
}

#endif
