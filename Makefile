
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
OBJECTS += obj/PassAstToCasmIR.o

INCLUDE += -I src
INCLUDE += -I lib/casm-fontend/src
INCLUDE += -I lib/casm-fontend/build/src

LIBRARY += lib/casm-fontend/build/libfrontend.a
LIBRARY += -lstdc++


default: obj $(TARGET)
#	$(CC) $(CF) $(CI) -c src/casmc.cpp -o obj/casmc.o
#	$(CC) $(CF) -o casmc obj/casmc.o lib/casm-fontend/build/libfrontend.a -lstdc++

obj:
	mkdir -p obj

obj/%.o: src/%.cpp
	$(CPP) $(CPPFLAG) $(INCLUDE) -c $^ -o $@

$(TARGET): $(OBJECTS)
	$(CPP) $(CPPFLAG) -o $@ $^ $(LIBRARY)

clean:
	rm -rf obj
	rm -f casmc

# libcasm-frontend
# libstdhl
# libstub

stub:
	PROJECT=casmc LICENSE=NSCA ./lib/stub/stub.sh cpp $(ARG) src
