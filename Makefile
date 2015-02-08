
#
# Copyright (c) 2015 Philipp Paulweber
# 
# This file is part of the 'casmc' project which is released under a NCSA
# open source software license. For more information, see the LICENSE.txt 
# file in the project root directory.
#


CC=gcc -std=c++11

INCLUDE=-I lib/casm-fontend/src -I lib/casm-fontend/build/src

default:
	$(CC) $(INCLUDE) -c src/main.cpp -o obj/main.o

	$(CC) -o casmc obj/main.o lib/casm-fontend/build/libfrontend.a -lstdc++
