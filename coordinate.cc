/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "coordinate.h"

#include <iostream>

std::ostream& Coordinate::put(std::ostream& os) const
{
    os << ne[0] << "\\~" << ne[1];
    if (radius.empty()) return os;

    os << R"(\~(\[t+-])" << radius << R"(\|m))";
    return os;
}
