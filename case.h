/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ETIKETT_CASE_H
#define ETIKETT_CASE_H

#include <string>

/**
 * Artportalen incorrectly capitalizes the first letter of common
 * names. Undo it, but take care with a handful of names (e.g. "Adam
 * och Eva", "Broddesons maskros") which do start with a capital.
 *
 * Necessarily incomplete: will lowercase some rarely-used names which
 * should begin with a capital.
 */
std::string fix_case(std::string name);

#endif
