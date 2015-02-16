
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _TYPECHECKPASS_H_
#define _TYPECHECKPASS_H_

#include "Pass.h"
#include "AstInterface.h"

#include "SourceToAstPass.h"

/**
   @file     TypeCheckPass.h
   @class    TypeCheckPass
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-16
*/

class TypeCheckPass : public Pass
{
public:
	static char id;
	
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/

	virtual bool run( PassResult& pr );
};

#endif /* _TYPECHECKPASS_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
