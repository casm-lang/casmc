
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

#include "version.h"

/**
    @brief TODO

    TODO

    @author   Philipp Paulweber
    @date     2015-01-27
*/

int main( int argc, const char *argv[] )
{
	const char* file_name = 0;
	
	Args options( argc, argv, Args::ALTERNATE, [&file_name,&options]( const char* arg ) 
	{
		static int cnt = 0;
		cnt++;
		
		if( cnt > 1 )
		{
			options.error( 1, "to many file names passed" );
		}
		
		file_name = arg;
	});
	
	options.add
	( 'h', "help", Args::NONE, "Display the program usage and synoptis"
	, [&options]( const char* option )
	{
		fprintf( stderr
		, "Corithian Abstract State Machine (CASM) Compiler\n"
		  "\n"
		  "usage: %s [options] <file>\n"
		  "\n"
		  "options:\n"
		, options.getProgramName()
		);
		
		options.usage();
		
		exit( 0 );
	});
	
	options.add( 'v', "-version", Args::NONE, "Display compiler version information"
	, [&options]( const char* option )
	{
		fprintf( stderr
		, "%s: version: %s [ %s %s ]\n"
		, options.getProgramName()
		, VERSION
		, __DATE__
		, __TIME__
		);
		
		exit( 0 );
	});
	
	for( auto& p : PassRegistry::getRegisteredPasses() )
	{
		//PassId    id = p.first;
		PassInfo& pi = *p.second;
		
		if( pi.getPassArgChar() == 0 && pi.getPassArgString() == 0 )
		{
			// internal pass, do not register a cmd line flag
			continue;
		}
		
		options.add
	   	( pi.getPassArgChar()
		, pi.getPassArgString()
		, Args::NONE
		, pi.getPassDescription()
		, [&pi]( const char* option )
		{
			printf( "add: %s:%s\n", pi.getPassName(), option );
			// add to PassManager the selected pass to run!
		});
	}
	
	options.parse();
	
	if( !file_name )
	{
		options.error( 1, "no input file provided" );
	}
	
	PassResult x;
	x.getResults()[ 0 ] = (void*)file_name;
	
	SourceToAstPass a;
	TypeCheckPass b;
	AstDumpPass c;
	AstToCasmIRPass d; 
	
	if( !a.run( x ) )
	{
		return -1;
	}
	
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
