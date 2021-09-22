# Makefile
#
# Copyright (c) 2018 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

CXXFLAGS=-W -Wall -pedantic -std=c++14 -g -Os
CPPFLAGS=-I/usr/include/libxml2
ARFLAGS=rTP

.PHONY: all
all: ap2csv
all: test/test

ap2csv: ap2csv.o libxlsx.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lxlsx -lxml2

libxlsx.a: strings.o
libxlsx.a: sheet.o
	$(AR) $(ARFLAGS) $@ $^

# tests

.PHONY:  checkv
check: test/test
	./test/test
checkv: test/test
	valgrind -q ./test/test -v

test/test: test/test.o test/libtest.a libxlsx.a
	$(CXX) $(CXXFLAGS) -o $@ test/test.o -Ltest/ -ltest -L. -lxlsx -lxml2

test/test.cc: test/libtest.a
	orchis -o $@ $^

test/libtest.a: test/foo.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

# other

.PHONY: install
install: ap2csv ap2csv.1
	install -m555 ap2csv $(INSTALLBASE)/bin/
	install -m644 ap2csv.1 $(INSTALLBASE)/man/man1/

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.{h,cc}

.PHONY: clean
clean:
	$(RM) ap2csv
	$(RM) *.o lib*.a
	$(RM) test/*.o test/lib*.a
	$(RM) test/test test/test.cc
	$(RM) -r dep
	$(RM) -r TAGS

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
