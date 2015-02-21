
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include "stdhl/cpp/Type.h"

class Pass;
class PassInfo;

/**
   @file     DataType.h
   @class    DataType
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

typedef Pass* (*PassConstructor)();
typedef void* PassId;
typedef void* Ptr;

typedef unordered_map< PassId, PassInfo* > PassId2PassInfo;
typedef unordered_map< PassId, void* > PassId2Ptr;
typedef unordered_map< PassId, u64 > PassId2u64;

namespace libpass {
class DataType  
{
public:
	
	
	
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/

};
}
#endif /* _DATATYPE_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
