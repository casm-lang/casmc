
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>
#include <assert.h>

#include <set>
#include <map>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Pass;
class PassInfo;

/**
   @file     Type.h
   @class    Type
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

typedef uint16_t u16;
typedef uint64_t u64;


typedef Pass* (*PassConstructor)();
typedef void* PassId;
typedef void* Ptr;

typedef unordered_map< PassId, PassInfo* > PassId2PassInfo;
typedef unordered_map< PassId, void* > PassId2Ptr;
typedef unordered_map< PassId, u64 > PassId2u64;

namespace pp {
class Type  
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
#endif /* _TYPE_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
