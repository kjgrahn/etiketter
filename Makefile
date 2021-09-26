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
libetiketter.a: case.o
libetiketter.a: stdin.o
	$(AR) $(ARFLAGS) $@ $^

libxlsx.a: xlsx.o
libxlsx.a: strings.o
libxlsx.a: sheet.o
libxlsx.a: utf8.o
libxlsx.a: join.o
	$(AR) $(ARFLAGS) $@ $^

etiketter: etiketter.o libetiketter.a libxlsx.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -letiketter -lxlsx -lxml2 -larchive

CFLAGS=-W -Wall -pedantic -ansi -g -Os
CXXFLAGS=-W -Wall -pedantic -std=c++14 -g -Os
CPPFLAGS=-I/usr/include/libxml2
ARFLAGS=rTP

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

libtest.a: test/split.o
libtest.a: test/key.o
libtest.a: test/thousands.o
libtest.a: test/coordinate.o
libtest.a: test/case.o
libtest.a: test/utf8.o
libtest.a: test/join.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

tests: tests.o libtest.a libetiketter.a libxlsx.a
	$(CXX) $(CXXFLAGS) -o $@ tests.o -L. -ltest -letiketter -lxlsx

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
	$(RM) *.o test/*.o lib*.a
	$(RM) tests.cc tests
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
