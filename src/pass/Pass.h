
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _PASS_H_
#define _PASS_H_

#include "PassInfo.h"
#include "PassUsage.h"
#include "PassResult.h"
#include "PassRegistry.h"

class PassUsage;
class PassResult;

/**
   @file     Pass.h
   @class    Pass
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

class Pass  
{
public:
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
	
    virtual void usage(PassUsage& pu)
	{
	}
	
	virtual void initialize(void)
	{
	}
	
	virtual bool run(PassResult& pr) = 0;
	
	virtual bool verify(void)
	{
		return true;
	}
	
	virtual void finalize(void)
	{
	}	
	
	virtual void dealloc(void* result)
	{
	}	
};

#endif /* _PASS_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
