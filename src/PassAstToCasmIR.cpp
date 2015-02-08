
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#include "PassAstToCasmIR.h"

/**
   @file     PassAstToCasmIR.cpp
   @author   Philipp Paulweber
   @date     2015-02-08
*/

PassAstToCasmIR::PassAstToCasmIR
( Driver& driver 
)
: ast_driver( driver )
{
}

// bool PassAstToCasmIR::visit_int_atom( IntAtom* node )
// { 
// 	printf( "%p\n", node ); 
// 	return 0; 
// }


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
