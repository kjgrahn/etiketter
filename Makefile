# Makefile
#
# Copyright (c) 2013, 2021 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash

all: etiketter
all: etiketter.1
all: tests

INSTALLBASE=/usr/local

libetiketter.a: split.o
libetiketter.a: key.o
libetiketter.a: thousands.o
libetiketter.a: coordinate.o
	$(AR) -r $@ $^

etiketter: etiketter.o libetiketter.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -letiketter

CFLAGS=-W -Wall -pedantic -ansi -g -Os
CXXFLAGS=-W -Wall -pedantic -std=c++11 -g -Os

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

libtest.a: test/split.o
libtest.a: test/key.o
libtest.a: test/thousands.o
	$(AR) -r $@ $^

test/%.o: CPPFLAGS+=-I.

tests: tests.o libtest.a libetiketter.a
	$(CXX) $(CXXFLAGS) -o $@ tests.o -L. -ltest -letiketter

tests.cc: libtest.a
	orchis -o $@ $^

.PHONY: install
install: all
	install -m555 etiketter $(INSTALLBASE)/bin/
	install -m644 etiketter.1 $(INSTALLBASE)/man/man1/

.PHONY: tags TAGS
tags: TAGS
TAGS:
	ctags -eR . --extra=q

.PHONY: clean
clean:
	$(RM) etiketter
	$(RM) *.o lib*.a
	$(RM) tests
	$(RM) -r dep

love:
	@echo "not war?"

# DO NOT DELETE

$(shell mkdir -p dep/test)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
