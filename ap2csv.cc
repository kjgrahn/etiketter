/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "sheet.h"

#include <iostream>

int main()
{
    const xlsx::SharedStrings ss {"test/xl/sharedStrings.xml"};
    const xlsx::Sheet sheet {"test/xl/worksheets/sheet1.xml", ss};

    auto& os = std::cout;
    for (const auto& row : sheet) {
	for (const auto& cell : row) {
	    os << cell << '\n';
	}
    }
    return 0;
}
