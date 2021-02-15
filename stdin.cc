/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "stdin.h"

#include "split.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <array>

namespace {

    char* next(char*& p)
    {
	while(*p) p++;
	p++;
	return p;
    }

    constexpr char nil = '\0';

    /**
     * Constructing the argv for execvp(2).
     */
    class Argv {
    public:
	explicit Argv(const std::vector<std::string>& ss);
	Argv(const Argv&) = delete;
	Argv& operator= (const Argv&) = delete;

	std::vector<char*> val;

    private:
	std::string buf;
    };

    Argv::Argv(const std::vector<std::string>& ss)
	: val {ss.size() + 1, nullptr},
	  buf {join(nil, ss)}
    {
	char* p = &buf.front();
	for (unsigned i = 0; i < ss.size(); i++) {
	    val[i] = p;
	    next(p);
	}
    }
}

Stdin::Stdin(const std::vector<std::string>& ss)
{
    int pipe[2];
    const int& rfd = pipe[0];
    const int& wfd = pipe[1];
    int err = pipe2(pipe, 0);
    if (err) {
	bork("pipe");
	return;
    }

    child = fork();
    if (child==-1) {
	bork("fork");
	return;
    }

    if (child) {
	close(0);
	close(wfd);
	dup(rfd);
	close(rfd);
	return;
    }

    // child

    close(0);
    open("/dev/null", O_RDONLY);
    close(1);
    close(rfd);
    dup(wfd);
    close(wfd);

    const Argv argv {ss};
    execvp(argv.val[0], argv.val.data());

    bork("exec", argv.val[0]);
    std::exit(1);
}

void Stdin::bork(const char* problem, const char* arg) const
{
    std::cerr << "Subprocess error at "
	      << problem << "('" << arg << "'): "
	      << std::strerror(errno) << '\n';
}
