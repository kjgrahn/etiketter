/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef XLSX_XLSX
#define XLSX_XLSX

#include "sheet.h"

#include <iosfwd>
#include <memory>

namespace xlsx {

    /**
     * Reading a single-sheet Excel document, row by row. The input is
     * supposed to be the kind of simple Microsoft Excel 2007
     * documents which are the export format of Artportalen.
     */
    class Source {
    public:
	explicit Source(std::istream& is);
	auto begin() const { return sheet->begin(); }
	auto end() const { return sheet->end(); }

    private:
	std::unique_ptr<Sheet> sheet;
    };
}

#endif
