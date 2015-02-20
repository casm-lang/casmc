
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#include "SourceToAstPass.h"

/**
   @file     SourceToAstPass.cpp
   @author   Philipp Paulweber
   @date     2015-02-16
*/

char SourceToAstPass::id = 0;

static PassRegistration< SourceToAstPass > PASS
( "Source To AST Pass"
, "parse the source code and generate an AST"
, 0
, 'P'
);

bool SourceToAstPass::run( PassResult& pr )
{
	const char* file_name = (const char*)pr.getResults()[ 0 ];
	
	casm_frontend_init();
	
	AstNode* node = casm_frontend_pass_1_parse( file_name );
	
    if( !node ) 
	{
		// TODO: PPA: better error messages!
        std::cerr << "Error parsing file" << std::endl;
		
		return false;
    }
	
	pr.setResult< SourceToAstPass >( node );
	
	return true;
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
