#   
#   Copyright (c) 2015 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/casmc
#   
#   Permission is hereby granted, free of charge, to any person obtaining a 
#   copy of this software and associated documentation files (the "Software"), 
#   to deal with the Software without restriction, including without limitation 
#   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
#   and/or sell copies of the Software, and to permit persons to whom the 
#   Software is furnished to do so, subject to the following conditions:
#   
#   * Redistributions of source code must retain the above copyright 
#     notice, this list of conditions and the following disclaimers.
#   
#   * Redistributions in binary form must reproduce the above copyright 
#     notice, this list of conditions and the following disclaimers in the 
#     documentation and/or other materials provided with the distribution.
#   
#   * Neither the names of the copyright holders, nor the names of its 
#     contributors may be used to endorse or promote products derived from 
#     this Software without specific prior written permission.
#   
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#   CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#   WITH THE SOFTWARE.
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
OBJECTS += obj/CasmIRToLLCodePass.o

INCLUDE += -I src
INCLUDE += -I src/ir
INCLUDE += -I obj
INCLUDE += -I lib/casm-frontend/src
INCLUDE += -I lib/casm-frontend/build/src
INCLUDE += -I lib/casm-ir/src
INCLUDE += -I lib/casm-rt/src
INCLUDE += -I lib/pass/src

INCLUDE += -I lib
#INCLUDE += -I lib/stdhl/c

LIBRARY += lib/casm-frontend/build/libfrontend.a
LIBRARY += lib/stdhl/libstdhlc.a
LIBRARY += lib/stdhl/libstdhlcpp.a
LIBRARY += lib/casm-ir/libcasm-ir.a
LIBRARY += lib/casm-rt/libcasm-rt.a


.PHONY: obj/version.h


default: $(TARGET)


all: clean doxy default

doxy:
	doxygen

obj:
	@echo "MKD " obj
	@mkdir -p obj

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

lib/casm-ir/libcasm-ir.a: lib/casm-ir
	@cd $<; $(MAKE)

lib/casm-rt/libcasm-rt.a: lib/casm-rt
	@cd $<; $(MAKE)

obj/version.h: obj
	@echo "GEN " $@ 
	@echo "#define VERSION \""`git describe --always --tags --dirty`"\"" > $@

$(TARGET): obj/version.h $(LIBRARY) $(OBJECTS)
	make llvm -C lib/stdll
	make -C lib/casm-ir
	make -C lib/casm-rt
	make llvm -C lib/casm-rt
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) -o $@ $(filter %.o,$^) $(filter %.a,$^) -lstdc++

clean:
	@echo "RMD " obj
	@rm -rf obj
	@echo "RM  " casmc
	@rm -f casmc
	$(MAKE) clean -C lib/casm-ir
	$(MAKE) clean -C lib/casm-rt

stub:
	PROJECT=casmc LICENSE=NSCA ./lib/stub/stub.sh cpp $(ARG) src



status:
	git status
	@for i in lib/*; do ( \
		echo "===----------------------------------------------------==="; \
		echo $$i; \
		cd $$i; \
		git status; \
		git describe --always --tags --dirty ); \
	done 

test-%:
	make; time ./casmc var/example/$*.casm -o out.ll; cat out.ll
	cat lib/casm-rt/casm-rt.ir out.ll > out.ir.ll
	llvm-link out.ir.ll lib/stdll/stdll.ll lib/casm-rt/casm-rt.ll -S -o out.cc.ll
	clang -g -O0 -o out.bin out.cc.ll
