
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _PASSREGISTRY_H_
#define _PASSREGISTRY_H_

#include "Type.h"

#include "PassInfo.h"

/**
   @file     PassRegistry.h
   @class    PassRegistry
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

class PassRegistry  
{
private:
	static PassId2PassInfo& registeredPasses(void)
	{
		static PassId2PassInfo passes;
		return passes;
	}
	
public:
    PassRegistry()
    {
		assert(0 && "PassRegistry class is a static-only non-object class!");
    }
	
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
	
	static void registerPass(PassInfo* passInfo)
	{
		assert( passInfo != 0 && "invalid pass info object pointer" );
		registeredPasses()[ passInfo->getPassId() ] = passInfo;
	
		// TODO: add checks for redundant argument names etc.
	}
	
	static PassId2PassInfo& getRegisteredPasses(void)
	{
		return registeredPasses();
	}
	
	static PassInfo& getPassInfo( PassId id )
	{
		PassInfo* pi  = static_cast<PassInfo*>( getRegisteredPasses()[ id ] );
		
		assert( pi != 0 && "invalid pass info object" );
		
		return *pi;
	}
};

template<class PassName>
Pass* defaultConstructor()
{
	return new PassName();
}

template<class PassName>
class PassRegistration : public PassInfo
{
public:
	PassRegistration(const char* name,
					 const char* description,
					 const char* argument,
					 const char* argument_short
					 )
		: PassInfo(name,
				   description,
				   argument,
				   argument_short,
				   &PassName::id,
				   PassConstructor( defaultConstructor<PassName>) ) 
	{
		PassRegistry::registerPass(this);
	}
};


#endif /* _PASSREGISTRY_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
