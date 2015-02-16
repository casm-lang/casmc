
#
# Copyright (c) 2015 Philipp Paulweber
# 
# This file is part of the 'casmc' project which is released under a NCSA
# open source software license. For more information, see the LICENSE.txt 
# file in the project root directory.
#

CPP=clang

CPPFLAG += -std=c++11
CPPFLAG += -g
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
INCLUDE += -I lib/casm-frontend/src
INCLUDE += -I lib/casm-frontend/build/src
INCLUDE += -I lib/stdhl/c
INCLUDE += -I lib/stdhl/cpp

LIBRARY += lib/casm-frontend/build/libfrontend.a
LIBRARY += lib/stdhl/libstdhlc.a
LIBRARY += lib/stdhl/libstdhlcpp.a

default: obj $(TARGET)
#	$(CC) $(CF) $(CI) -c src/casmc.cpp -o obj/casmc.o
#	$(CC) $(CF) -o casmc obj/casmc.o lib/casm-frontend/build/libfrontend.a -lstdc++

obj:
	mkdir -p obj

obj/%.o: src/%.cpp
	@echo "CC " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

obj/%.o: src/%.c
	@echo "CC " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

lib/casm-frontend/build/libfrontend.a: lib/casm-frontend
	cd $<; make

lib/stdhl/libstdhlc.a: lib/stdhl
	cd $<; make

lib/stdhl/libstdhlcpp.a: lib/stdhl
	cd $<; make

$(TARGET): $(LIBRARY) $(OBJECTS)
	@echo "LK " $@
	@$(CPP) $(CPPFLAG) -o $@ $(filter %.o,$^) $(filter %.a,$^) -lstdc++

clean:
	rm -rf obj
	rm -f casmc

stub:
	PROJECT=casmc LICENSE=NSCA ./lib/stub/stub.sh cpp $(ARG) src
