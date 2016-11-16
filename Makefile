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

CPP=clang

CPPFLAG += -std=c++11
CPPFLAG += -g -O0
CPPFLAG += -Wall
#CPPFLAG += -Wextra

TARGET=casmc

OBJECTS += obj/casmc.o
OBJECTS += obj/PassManager.o
#OBJECTS += obj/SourceToAstPass.o
#OBJECTS += obj/TypeCheckPass.o
#OBJECTS += obj/AstDumpPass.o
#OBJECTS += obj/AstToCasmIRPass.o
#OBJECTS += obj/CasmIRToLLCodePass.o

INCLUDE += -I src
INCLUDE += -I src/ir
INCLUDE += -I obj
INCLUDE += -I lib/stdhl
INCLUDE += -I lib/pass
INCLUDE += -I lib/casm-fe
INCLUDE += -I lib/casm-ir
INCLUDE += -I lib/casm-be
INCLUDE += -I lib/casm-rt
INCLUDE += -I lib/casm-tc
INCLUDE += -I lib/novel

INCLUDE += -I lib/z3/src/api
INCLUDE += -I lib/z3/src/api/c++

LIBRARY  = lib/stdhl/libstdhlc.a
LIBRARY += lib/stdhl/libstdhlcpp.a
LIBRARY += lib/casm-fe/libcasm-fe.a
LIBRARY += lib/casm-ir/libcasm-ir.a
LIBRARY += lib/casm-be/libcasm-be.a
LIBRARY += lib/casm-rt/libcasm-rt.a
LIBRARY += lib/novel/libnovel.a

#LIBRARY += lib/z3/build/libz3.a


.PHONY: obj/version.h obj/license.h $(LIBRARY)


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


lib/stdhl/libstdhlc.a lib/stdhl/libstdhlcpp.a: lib/stdhl
	@cd $<; $(MAKE)

lib/casm-fe/libcasm-fe.a: lib/casm-fe
	@cd $<; $(MAKE)

lib/casm-ir/libcasm-ir.a: lib/casm-ir
	@cd $<; $(MAKE)

lib/casm-rt/libcasm-rt.a: lib/casm-rt
	@cd $<; $(MAKE)

lib/casm-be/libcasm-be.a: lib/casm-be
	@cd $<; $(MAKE)

lib/novel/libnovel.a: lib/novel
	@cd $<; $(MAKE)

obj/version.h: obj
	@echo "GEN " $@ 
	@echo "#define VERSION \""`git describe --always --tags --dirty`"\"" > $@

obj/license.h: obj
	@echo "GEN " $@
	head -n 19 LICENSE.txt > $@.txt
	echo "#define LICENSE \\" > $@
	while IFS= read -r line; do echo "\"    $$line\n\" \\"; done < $@.txt >> $@

$(TARGET): obj/version.h obj/license.h $(LIBRARY) $(OBJECTS)
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) -o $@ $(filter %.o,$^) $(filter %.a,$^) lib/z3/build/libz3.so -lstdc++ -lm

clean:
	@echo "RMD " obj
	@rm -rf obj
	@echo "RM  " casmc
	@rm -f casmc
	$(MAKE) clean -C lib/casm-fe
	$(MAKE) clean -C lib/casm-ir
	$(MAKE) clean -C lib/casm-be
	$(MAKE) clean -C lib/casm-rt
	$(MAKE) clean -C lib/novel


git-%:
	git $*
	@for i in lib/*; do ( \
		echo "===----------------------------------------------------==="; \
		echo $$i; \
		cd $$i; \
		git $*; \
		git describe --always --tags --dirty ); \
	done 

test-%:
	make; time ./casmc var/example/$*.casm -o obj/out.ll; cat obj/out.ll
	cat lib/casm-rt/casm-rt.ir obj/out.ll > obj/out.ir.ll
	llvm-link -S -o obj/out.cc.ll obj/out.ir.ll
#	clang -g -flto -S -o obj/out.cc.ll obj/out.ir.ll
#	llvm-link obj/out.ir.ll lib/stdll/stdll.ll lib/casm-rt/casm-rt.ll -S -o obj/out.cc.ll
	clang -g -O0 -o obj/out.bin    obj/out.cc.ll
	clang    -O1 -o obj/out.bin.O1 obj/out.cc.ll
	clang    -O2 -o obj/out.bin.O2 obj/out.cc.ll
	clang    -O3 -o obj/out.bin.O3 obj/out.cc.ll

	opt -O3 -S -o obj/out.opt.ll obj/out.ir.ll
	clang -O3 -o obj/out.bin.Oo obj/out.opt.ll

attic-%:
	cat lib/casm-rt/casm-rt.ir .attic/$*.ll > obj/out.ir.ll
	llvm-link -S -o obj/out.cc.ll obj/out.ir.ll
#	clang -g -flto -S -o obj/out.cc.ll obj/out.ir.ll
#	llvm-link obj/out.ir.ll lib/stdll/stdll.ll lib/casm-rt/casm-rt.ll -S -o obj/out.cc.ll
	clang -g -O0 -o obj/out.bin    obj/out.cc.ll
	clang    -O1 -o obj/out.bin.O1 obj/out.cc.ll
	clang    -O2 -o obj/out.bin.O2 obj/out.cc.ll
	clang    -O3 -o obj/out.bin.O3 obj/out.cc.ll

	opt -O3 -S -o obj/out.opt.ll obj/out.ir.ll
	clang -O3 -o obj/out.bin.Oo obj/out.opt.ll


opt:
	@echo OPTS=$(OPTS)
	@for i in \
	\
	adce add-discriminators always-inline argpromotion atomic-ll-sc \
	barrier basicaa basiccg bb-vectorize block-freq bounds-checking branch-prob break-crit-edges \
	codegenprepare consthoist constmerge constprop correlated-propagation cost-model count-aa \
	da dce deadargelim delinearize die domfrontier domtree dse \
	early-cse \
	functionattrs \
	globaldce globalopt globalsmodref-aa gvn \
	indvars inline inline-cost instcombine instcount instnamer instsimplify intervals ipconstprop ipsccp iv-users \
	jump-instr-table-info jump-threading \
	lazy-value-info lcssa libcall-aa licm load-combine \
	loop-deletion loop-extract loop-extract-single loop-idiom loop-instsimplify \
	loop-reduce loop-reroll loop-rotate loop-simplify loop-unroll loop-unswitch loop-vectorize loops \
	lower-expect loweratomic lowerinvoke lowerswitch \
	mem2reg memcpyopt memdep mergefunc mergereturn mldst-motion module-debuginfo \
	no-aa notti \
	partially-inline-libcalls postdomtree prune-eh \
	reassociate reg2mem regions \
	scalar-evolution scalarizer scalarrepl scalarrepl-ssa sccp scev-aa \
	separate-const-offset-from-gep simplifycfg sink slp-vectorizer sroa \
	strip strip-dead-debug-info strip-dead-prototypes strip-debug-declare strip-nondebug structurizecfg \
	tailcallelim targetlibinfo tbaa \
	verify \
	; do \
	  rm -f obj/out.opt.ll; \
	  opt \
	  -mtriple=x86_64-pc-linux-gnu -mcpu=core2 -mattr=+avx2 \
	  $(OPTS) \
	  -$$i \
	  -S -o obj/out.opt.ll obj/out.ir.ll; 2> /dev/null \
	  clang -O0 -o obj/out.bin.opt obj/out.opt.ll 2> /dev/null; \
	  for c in 1 2 3; do \
	    printf "%-30s, " $$i; \
	    /usr/bin/time -f "%e, W=%W, c=%c, w=%w, F=%F, R=%R, I=%I, O=%O, k=%k" ./obj/out.bin.opt > /dev/null; \
	  done; \
	done

opt-%:
	opt -mtriple=x86_64-pc-linux-gnu -mcpu=core2 -mattr=+avx2 \
	$* \
	-S -o obj/out.OPT.ll obj/out.ir.ll; 2> /dev/null
	@for c in 0 1 2 3; do \
	  @clang -O$$c -o obj/out.bin.OPT obj/out.OPT.ll 2> /dev/null; \
	  /usr/bin/time -f "%e, W=%W, c=%c, w=%w, F=%F, R=%R, I=%I, O=%O, k=%k" ./obj/out.bin.OPT > /dev/null; \
	done

nop:
	opt -S -o obj/out.OPT.ll obj/out.ir.ll; 2> /dev/null
	@for c in 0 0 0 1 1 1 2 2 2 3 3 3; do \
	  @clang -O$$c -o obj/out.bin.OPT obj/out.OPT.ll 2> /dev/null; \
	  /usr/bin/time -f "%e, W=%W, c=%c, w=%w, F=%F, R=%R, I=%I, O=%O, k=%k" ./obj/out.bin.OPT > /dev/null; \
	done

# asan asan-module dfsan
# partial-inliner sample-profile 

# internalize tsan msan metarenamer 

# O1 O2 O3
# aa-eval lint

