
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#include "AstDumpPass.h"

/**
   @file     AstDumpPass.cpp
   @author   Philipp Paulweber
   @date     2015-02-16
*/

char AstDumpPass::id = 0;

static libpass::PassRegistration< AstDumpPass > PASS
( "AST Dumping Pass"
, "generates a DOT graph of the AST"
, "ast-dump"
, 0
);

bool AstDumpPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	AstWalker< AstDumpVisitor, bool > dump_walker( *this );
	
	dump_walker.walk_specification( node );
	
	std::cout << this->get_dump() << std::endl;
	
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
