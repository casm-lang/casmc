//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/casmc
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  WITH THE SOFTWARE.
//  

#include "stdhl/cpp/Default.h"
#include "stdhl/cpp/Args.h"

#include "Pass.h"

#include "AstDumpPass.h"
#include "AstToCasmIRPass.h"
#include "CasmIRToLLCodePass.h"

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
	const char* output_name = 0;

	Args options( argc, argv, Args::ALTERNATE
	, [&file_name,&options]( const char* arg ) 
	{
		static int cnt = 0;
		cnt++;
		
		if( cnt > 1 )
		{
			options.error( 1, "to many file names passed" );
		}
		
		file_name = arg;
	});
	
	options.add( 'o', 0, Args::REQUIRED, "Place the output into <file>"
	, [&options,&output_name]( const char* option )
	{
		static int cnt = 0;
		cnt++;
		
		if( cnt > 1 )
		{
			options.error( 1, "to many output names passed" );
		}
		
		output_name = option;
	}
	, "file");

#define DESCRIPTION											\
	"Corinthian Abstract State Machine (CASM) Compiler\n"
	
	options.add
	( 'h', "help", Args::NONE, "Display the program usage and synoptis"
	, [&options]( const char* option )
	{
		fprintf( stderr
		, DESCRIPTION
		  "\n"
		  "usage: %s [options] <file>\n"
		  "\n"
		  "options:\n"
		, options.getProgramName()
		);
		
		options.usage();
		
		exit( 0 );
	});
	
	options.add( 'v', "version", Args::NONE, "Display compiler version information"
	, [&options]( const char* option )
	{
		fprintf( stderr
		, DESCRIPTION
		  "\n"
		  "%s: version: %s [ %s %s ]\n"
		, options.getProgramName()
		, VERSION
		, __DATE__
		, __TIME__
		);
		
		exit( 0 );
	});
	
	for( auto& p : libpass::PassRegistry::getRegisteredPasses() )
	{
		//PassId    id = p.first;
		libpass::PassInfo& pi = *p.second;
		
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
	
	libpass::PassResult x;
	x.getResults()[ 0 ] = (void*)file_name;
	x.getResults()[ (void*)1 ] = (void*)output_name;
	
	SourceToAstPass a;
	TypeCheckPass b;
	AstDumpPass c;
	AstToCasmIRPass d; 
    CasmIRToLLCodePass e; 
	
	if( !a.run( x ) )
	{
		return -1;
	}
	
	b.run( x );
	
	c.run( x );
	
	d.run( x );

	e.run( x );
	
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
	

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
