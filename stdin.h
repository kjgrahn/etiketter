/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ETIKETT_STDIN_H
#define ETIKETT_STDIN_H

#include <string>
#include <vector>

/**
 * Using fork/exec/pipe to replace fd 0 with a program, feeding us
 * with data.  So it's a way to make the command
 *
 *   % foo
 *
 * behave as if it was invoked as
 *
 *   % source args | foo
 *
 * The source will have
 *
 *   stdin   /dev/null
 *   stdout  a pipe
 *   stderr  shared with the parent
 *
 * I'm assuming this breaks neither std::stdin nor std::cin (they
 * cannot detect that fd 0 has been switched). That requires, though,
 * that std::cin has not been /used/.
 */
class Stdin {
public:
    explicit Stdin(const std::vector<std::string>& argv);

private:
    void bork(const char* problem, const char* arg = "") const;
    int child = 0;
};

#endif
