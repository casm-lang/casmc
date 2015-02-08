
/*
  Copyright (c) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NCSA
  open source software license. For more information, see the LICENSE.txt 
  file in the project root directory.
*/

#include <assert.h>

#include "frontend.h"
#include "libsyntax/driver.h"
#include "libsyntax/ast_dump_visitor.h"

/**
    @brief TODO

    TODO

    @author   Philipp Paulweber
    @date     2015-01-27
*/

extern Driver *global_driver;

int main( int argc, char *argv[] )
{
	// parsing & type-checking
	
    int res = 0;
	
	casm_frontend_init();
	
    if( argc != 2 )
	{
        std::cerr << "Error: invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }
	
	AstNode* ast = casm_frontend_pass_1_parse( argv[1] );
	
    if( ast == nullptr ) 
	{
        std::cerr << "Error parsing file" << std::endl;
        res = EXIT_FAILURE;
    } 
	else
	{
		if( !casm_frontend_pass_2_typecheck( ast ) )
		{
			std::cerr << "Error typecheck file" << std::endl;
			res = EXIT_FAILURE;    
		}
		
        res = EXIT_SUCCESS;
    }
	
	assert( global_driver && "invalid pointer" );
	
	AstDumpVisitor dump;
	AstWalker< AstDumpVisitor, bool > dump_walker( dump );
	dump_walker.walk_specification( ast );
	std::cout << dump.get_dump() << std::endl;
	
    casm_frontend_destroy();
	
	
	// transform CASM AST -> IR
	
	// optimizations (analysis and transformations)
	
	// interpretation: CASM-based
	
	// code-generation: CASM IR --+-> LLVM IR -> exec
	//                            |
	//                            +-> LLVM IR (interpretation via lli)
	
    return res;
}

/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
