
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

#include "PassAstToCasmIR.h"

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
	
	const char* file_name = argv[1];
	
	AstNode* ast = casm_frontend_pass_1_parse( file_name );
	
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
	
	if( res != EXIT_SUCCESS )
	{
		return res;
	}
	
	assert( global_driver && "invalid pointer" );
	
	AstDumpVisitor dump;
	AstWalker< AstDumpVisitor, bool > dump_walker( dump );
	dump_walker.walk_specification( ast );
	std::cout << dump.get_dump() << std::endl;
	
	std::string fn( file_name );
	fn += ".dot";
	
	std::ofstream fd;
    fd.open( fn );
	fd << dump.get_dump();
	fd.close();
	
	std::string cmd;
	cmd += "dot -Tpdf ";
	cmd += fn;
	cmd += " -o ";
	cmd += fn;
	cmd += ".pdf";
	
	system( cmd.c_str() );
	
	// ---
	
	PassAstToCasmIR ast_to_casm_ir( *global_driver );
	AstWalker< PassAstToCasmIR, bool > ast_to_casm_ir_walker( ast_to_casm_ir );
	ast_to_casm_ir_walker.walk_specification( ast );
	
	
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
