
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
OBJECTS += obj/AstToCasmIRPass.o
OBJECTS += obj/ExamplePass.o

INCLUDE += -I src
INCLUDE += -I lib/casm-frontend/src
INCLUDE += -I lib/casm-frontend/build/src

LIBRARY += lib/casm-frontend/build/libfrontend.a
LIBRARY += -lstdc++


default: obj $(TARGET)
#	$(CC) $(CF) $(CI) -c src/casmc.cpp -o obj/casmc.o
#	$(CC) $(CF) -o casmc obj/casmc.o lib/casm-frontend/build/libfrontend.a -lstdc++

obj:
	mkdir -p obj

obj/%.o: src/%.cpp
	@echo "CC " $^
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $^ -o $@

$(TARGET): $(OBJECTS)
	@echo "LK " $@
	@$(CPP) $(CPPFLAG) -o $@ $^ $(LIBRARY)

clean:
	rm -rf obj
	rm -f casmc


# libcasm-frontend
# libstdhl
# libstub

stub:
	PROJECT=casmc LICENSE=NSCA ./lib/stub/stub.sh cpp $(ARG) src
