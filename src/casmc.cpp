
/*
  Copyright (c) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NCSA
  open source software license. For more information, see the LICENSE.txt 
  file in the project root directory.
*/

#include "Default.h"
#include "Args.h"

#include "Pass.h"
#include "DataType.h"

#include "AstDumpPass.h"
#include "AstToCasmIRPass.h"

/**
    @brief TODO

    TODO

    @author   Philipp Paulweber
    @date     2015-01-27
*/

#include "args.h"


//void bla( void (*fun)(int, int) )
void bla( function< void( int, int ) > fun )
{
	fun( 1, 2);
}

int main( int argc, char *argv[] )
{
	args( argc, argv );
	
	bla( [](int x, int y)
	{ 
		x = x + y; 
	});
	
	for( auto& p : PassRegistry::getRegisteredPasses() )
	{
		PassId    id = p.first;
		PassInfo* pi = p.second;
		
		printf( "%p, %p, %s, --%s, -%s\n", 
				id, pi, pi->getPassName(), pi->getPassArgument(), pi->getPassArgumentShort() );
	}
	
    if( argc != 2 )
	{
        std::cerr << "Error: invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
    }
	
	const char* file_name = argv[1];
	
	// ---
	
	PassResult x;
	x.getResults()[ 0 ] = (void*)file_name;
	
	SourceToAstPass a;
	TypeCheckPass b;
	AstDumpPass c;
	AstToCasmIRPass d; 
	
	a.run( x );
	b.run( x );
	c.run( x );
	d.run( x );
	
	// transform CASM AST -> IR
	
	// optimizations (analysis and transformations)
	
	// interpretation: CASM-based
	
	// code-generation: CASM IR --+-> LLVM IR -> exec
	//                            |
	//                            +-> LLVM IR (interpretation via lli)
	
    return 0;
}

    // std::string fn( file_name );
	// fn += ".dot";
	
	// std::ofstream fd;
    // fd.open( fn );
	// fd << dump.get_dump();
	// fd.close();
	
	// std::string cmd;
	// cmd += "dot -Tpdf ";
	// cmd += fn;
	// cmd += " -o ";
	// cmd += fn;
	// cmd += ".pdf";
	
	// system( cmd.c_str() );
	

/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
