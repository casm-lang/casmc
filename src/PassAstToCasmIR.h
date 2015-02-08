
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _PASSASTTOCASMIR_H_
#define _PASSASTTOCASMIR_H_

#include "libsyntax/driver.h"

#include "AstInterface.h"

/**
   @file     PassAstToCasmIR.h
   @class    PassAstToCasmIR
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-08
*/


class PassAstToCasmIR : public AstInterface< bool >
{
private:
	
    Driver& ast_driver;

public:

    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
	
    PassAstToCasmIR( Driver& driver );
		
};

#endif /* _PASSASTTOCASMIR_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
