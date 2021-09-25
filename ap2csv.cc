/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "xlsx.h"

#include <iostream>
#include <fstream>

int main()
{
    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::ifstream in {"test/ExcelExport_1.xlsx"};
    xlsx::csv(std::cout, in);
    return 0;
}
