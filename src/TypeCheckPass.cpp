
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#include "TypeCheckPass.h"

/**
   @file     TypeCheckPass.cpp
   @author   Philipp Paulweber
   @date     2015-02-16
*/

char TypeCheckPass::id = 0;

static PassRegistration< TypeCheckPass > PASS
( "Type Check Pass"
, "internal: checks the AST and translates it to a typed AST"
, 0
, 0
);

bool TypeCheckPass::run( PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< SourceToAstPass >();
	
	if( !casm_frontend_pass_2_typecheck( node ) )
	{
		// TODO: PPA: better error message!
		std::cerr << "Error typecheck file" << std::endl;
		
		return false;
	}

	pr.setResult< TypeCheckPass >( node );
	
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
