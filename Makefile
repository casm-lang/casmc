
#
# Copyright (c) 2015 Philipp Paulweber
# 
# This file is part of the 'casmc' project which is released under a NCSA
# open source software license. For more information, see the LICENSE.txt 
# file in the project root directory.
#

CPP=clang

CPPFLAG += -std=c++11
CPPFLAG += -g -O0
CPPFLAG += -Wall
#CPPFLAG += -Wextra

TARGET=casmc

OBJECTS += obj/casmc.o
OBJECTS += obj/PassManager.o

OBJECTS += obj/SourceToAstPass.o
OBJECTS += obj/TypeCheckPass.o
OBJECTS += obj/AstDumpPass.o
OBJECTS += obj/AstToCasmIRPass.o

INCLUDE += -I src
INCLUDE += -I obj
INCLUDE += -I lib/casm-frontend/src
INCLUDE += -I lib/casm-frontend/build/src
#INCLUDE += -I lib/stdhl/c
INCLUDE += -I lib

LIBRARY += lib/casm-frontend/build/libfrontend.a
LIBRARY += lib/stdhl/libstdhlc.a
LIBRARY += lib/stdhl/libstdhlcpp.a

default: obj $(TARGET)
#	$(CC) $(CF) $(CI) -c src/casmc.cpp -o obj/casmc.o
#	$(CC) $(CF) -o casmc obj/casmc.o lib/casm-frontend/build/libfrontend.a -lstdc++

.PHONY: obj/version.h

all:
	$(MAKE) clean
	$(MAKE)

obj:
	mkdir -p obj

obj/%.o: src/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

obj/%.o: src/%.c
	@echo "CC  " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

lib/casm-frontend/build/libfrontend.a: lib/casm-frontend
	@cd $<; $(MAKE)

lib/stdhl/libstdhlc.a lib/stdhl/libstdhlcpp.a: lib/stdhl
	@cd $<; $(MAKE)

obj/version.h:
	@echo "GEN " $@ 
	@echo "#define VERSION \""`git describe --always --tags --dirty`"\"" > $@

$(TARGET): obj/version.h $(LIBRARY) $(OBJECTS)
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) -o $@ $(filter %.o,$^) $(filter %.a,$^) -lstdc++

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM " casmc
	@rm -f casmc

stub:
	PROJECT=casmc LICENSE=NSCA ./lib/stub/stub.sh cpp $(ARG) src
