/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ETIKETT_COORDINATE_H
#define ETIKETT_COORDINATE_H

#include "key.h"

#include <string>
#include <iosfwd>

/**
 * Coordinates, in Artportalen context, may show up in several
 * formats: RT90, SWEREF 99, and something else. They also tend to
 * show at least the first two in the wrong order: east--north instead
 * of north--east.
 *
 * This class doesn't do much, but is a place to focus anything that
 * /does/ need to be done.
 */
class Coordinate {
public:
    Coordinate(const Record::spair& ne, const std::string& radius)
	: ne {ne},
	  radius {radius}
    {}
    std::ostream& put(std::ostream& os) const;

private:
    const Record::spair ne;
    const std::string radius;
};

inline
std::ostream& operator<< (std::ostream& os, const Coordinate& val) { return val.put(os); }

#endif
