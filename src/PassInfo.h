
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _PASSINFO_H_
#define _PASSINFO_H_

#include "DataType.h"

/**
   @file     PassInfo.h
   @class    PassInfo
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

class PassInfo
{
private:
	const char* const name;
	const char* const description;
	const char* const argument;
	const char* const argument_short;
	
	const PassId  id;
	
	PassConstructor constructor;
	
	uint64_t changes;

public:
    PassInfo(const char* passName, 
			 const char* passDescription, 
			 const char* passArgument, 
			 const char* passArgumentShort, 
			 const PassId passID, 
			 PassConstructor passConstructor)
		: name(passName),
		  description(passDescription),
		  argument(passArgument),
		  argument_short(passArgumentShort),
		  id(passID),
		  constructor(passConstructor)
	{}
	
	/**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
	const char* getPassName(void) const 
	{ 
		return name; 
	}
	
	const char* getPassDescription(void) const 
	{ 
		return description; 
	}
	
	const char* getPassArgument(void) const 
	{ 
		return argument; 
	}
	
	const char* getPassArgumentShort(void) const 
	{ 
		return argument_short; 
	}
	
	const PassId getPassId(void) const
	{
		return id;
	}

	bool isPassID(const PassId passID) const
	{
		return passID == id;
	}
	
	void addChanges(uint64_t change)
	{
		changes += change;
	}
	
	uint64_t getChanges(void)
	{
		return changes;
	}
	
	Pass* constructPass(void) const
	{
		return constructor();
	}
	
	friend ostream& operator<<(ostream& os, PassInfo& pi)
	{
		return os; // << pi.getPassName() << ": ";
	}
};

#endif /* _PASSINFO_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/