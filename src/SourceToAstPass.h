
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _SOURCETOASTPASS_H_
#define _SOURCETOASTPASS_H_

#include "Pass.h"
#include "AstInterface.h"

/**
   @file     SourceToAstPass.h
   @class    SourceToAstPass
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-16
*/

class SourceToAstPass : public libpass::Pass
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
	
	virtual bool run( libpass::PassResult& pr );
};

#endif /* _SOURCETOASTPASS_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
