
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _ASTDUMPPASS_H_
#define _ASTDUMPPASS_H_

#include "AstInterface.h"
#include "Pass.h"
#include "TypeCheckPass.h"

/**
   @file     AstDumpPass.h
   @class    AstDumpPass
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-16
*/

class AstDumpPass : public libpass::Pass, AstDumpVisitor
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

#endif /* _ASTDUMPPASS_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
