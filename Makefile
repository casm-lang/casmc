#
#   Copyright (c) 2014-2016 CASM Organization https://github.com/casm-lang
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 Florian Hahn
#                 https://github.com/casm-lang/casmc
#
#   This file is part of casmc.
#
#   casmc is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   casmc is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with casmc. If not, see <http://www.gnu.org/licenses/>.
#

.PHONY: obj/version.h obj/license.h
.NOTPARALLEL: obj/version.h obj/license.h

default: debug

help:
	@echo "TODO"

REPO = casmc

TARGET = $(REPO)

TEST_TARGET = test-$(REPO)


CP  = $(shell find src -name '*.cpp' | cut -d'.' -f1)
CO  = $(CP:%=obj/%.o)

CI  = -I src
CI += -I obj
CI += -I lib/stdhl
CI += -I lib/pass
CI += -I lib/casm-fe
CI += -I lib/casm-ir
CI += -I lib/casm-tc
CI += -I lib/casm-be
CI += -I lib/casm-rt
CI += -I lib/csel-ir
CI += -I lib/csel-be
CI += -I lib/z3/src/api

CL  = obj/casmc.a
CL += lib/pass/libpass.a
CL += lib/stdhl/libstdhlcpp.a
CL += lib/casm-fe/libcasm-fe.a
CL += lib/casm-ir/libcasm-ir.a
CL += lib/casm-be/libcasm-be.a
CL += lib/casm-rt/libcasm-rt.a
CL += lib/csel-ir/libcsel-ir.a
CL += lib/csel-be/libcsel-be.a
CL += lib/z3/build/libz3.so

CC  =
CF  =

  %-gcc: CC = gcc
%-clang: CC = clang

  debug-%: CF += -O0 -g
release-%: CF += -O3 -DNDEBUG

linux%:  CF += -Wall -std=c++11
linux%:  XF += -Wall -std=c11
linux3%: CF += -m32
linux6%: CF += -m64


build: config $(TARGET)
check: build $(TEST_TARGET)

linux32-build: build
linux64-build: build

linux32-check: check
linux64-check: check


  debug-build-linux32-gcc:   linux32-build
  debug-check-linux32-gcc:   linux32-check
release-build-linux32-gcc:   linux32-build
release-check-linux32-gcc:   linux32-check

  debug-build-linux64-gcc:   linux64-build
  debug-check-linux64-gcc:   linux64-check
release-build-linux64-gcc:   linux64-build
release-check-linux64-gcc:   linux64-check

  debug-build-linux32-clang: linux32-build
  debug-check-linux32-clang: linux32-check
release-build-linux32-clang: linux32-build
release-check-linux32-clang: linux32-check

  debug-build-linux64-clang: linux64-build
  debug-check-linux64-clang: linux64-check
release-build-linux64-clang: linux64-build
release-check-linux64-clang: linux64-check


  debug:   debug-build-linux64-clang
release: clean release-build-linux64-clang

test:           debug-check-linux64-clang
test-release: release-check-linux64-clang


all: clean default

#doxy:
#	doxygen

config: CFG="CC=$(CC) CF=\"$(CF)\" '$(MAKE) $(MFLAGS)'"
config:
	@echo "CFG  $(CFG)"


obj/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

obj/%.o: %.c
	@mkdir -p `dirname $@`
	@echo "C   " $<
	@$(CC) $(CF) $(CI) -c $< -o $@



lib/pass/libpass.a: lib/pass
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/stdhl/libstdhlcpp.a: lib/stdhl
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/casm-fe/libcasm-fe.a: lib/casm-fe
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/casm-ir/libcasm-ir.a: lib/casm-ir
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/casm-rt/libcasm-rt.a: lib/casm-rt
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/casm-be/libcasm-be.a: lib/casm-be
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/csel-ir/libcsel-ir.a: lib/csel-ir
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"

lib/csel-be/libcsel-be.a: lib/csel-be
	@cd $<; $(MAKE) $(MFLAGS) build CC="$(CC)" CF="$(CF)"


obj/version.h:
	@mkdir -p `dirname $@`
	@echo "GEN " $@ 
	@echo "#define VERSION \""`git describe --always --tags --dirty`"\"" > $@

obj/license.h:
	@mkdir -p `dirname $@`
	@echo "GEN " $@
	echo "#define LICENSE \"TBD\"" > $@

obj/casmc.a: $(CO)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
	@ranlib $@


$(TARGET): obj/version.h obj/license.h $(CL)
	@echo "LD  " $@
	@$(CC) $(CF) -o $@ $(CL) -lstdc++ -lm

clean:
	$(MAKE) $(MFLAGS) clean -C lib/pass
	$(MAKE) $(MFLAGS) clean -C lib/stdhl
	$(MAKE) $(MFLAGS) clean -C lib/casm-fe
	$(MAKE) $(MFLAGS) clean -C lib/casm-ir
	$(MAKE) $(MFLAGS) clean -C lib/casm-be
	$(MAKE) $(MFLAGS) clean -C lib/csel-ir
	$(MAKE) $(MFLAGS) clean -C lib/csel-be
	@echo "RMD " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)
	@echo "RM  " $(TEST_TARGET)
	@rm -f $(TEST_TARGET)


TF   = $(shell find uts -name '*.cpp' | cut -d'.' -f1)
TO = $(TF:%=obj/%.o)

TI  = -I lib/gtest/googletest/include
TI += -I lib/gtest/googletest
TS  = lib/gtest/googletest/src/gtest-all.cc
TS += lib/gtest/googletest/src/gtest_main.cc

TL  = -lstdc++
TL += -lm
TL += -lpthread

obj/uts/%.o: uts/%.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(TI) $(CI) -c $< -o $@

$(TEST_TARGET): $(TO) $(CO) $(CL) $(TARGET)
	@echo "LD  " $@
	@$(CC) \
	  $(CF) \
	  $(TI) \
	  $(CI) \
	  $(TL) \
	  -o $@ \
	  $(TO) \
	  $(TS)
	@echo "RUN " $@
	@./$@
	@echo "RUN " $@
	CASM=`pwd`/$(TARGET) $(MAKE) $(MFLAGS) test-run -C lib/casm-tc


info:
	@echo $(CWD) 

# # test: $(TARGET) $(TEST_TARGET) default
# # 	@rm -f $@
# # 	@echo "LD  " $@
# # 	@$(CC) $(CF) $(TI) $(CI) $(TL) -o $@ \
# # 		-Wl,--whole-archive $(TEST_TARGET) $(TARGET) -Wl,--no-whole-archive \
# # 		 ../gtest/googletest/src/gtest-all.cc ../gtest/googletest/src/gtest_main.cc 
# # 	@echo "RUN " $@
# # 	@./$@
