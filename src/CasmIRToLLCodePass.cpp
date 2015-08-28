//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/casmc
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  WITH THE SOFTWARE.
//  

#include "CasmIRToLLCodePass.h"


char CasmIRToLLCodePass::id = 0;

static libpass::PassRegistration< CasmIRToLLCodePass > PASS
( "CASM IR to LLVM IR"
, "generates LLVM IR code of CASM IR"
, 0
, 0
);

static const char* default_output_name = "stdout";

using namespace libcasm_ir;

bool CasmIRToLLCodePass::run( libpass::PassResult& pr )
{
	// AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	const char* output_name = (const char*)pr.getResults()[ (void*)1 ];
	FILE* output = stdout;
	if( !output_name )
	{
		output_name = default_output_name;
	}
	else
	{
		output = fopen( output_name, "w" );
	}
	
	printf( "%s:%i: [%s] '%s'\n", __FILE__, __LINE__, __FUNCTION__, output_name );
	
	
	Value::SymbolTable& symbols = *Value::getSymbols();

	fprintf( output, "\n" );
	fprintf( output, "; constants\n" );
	
	for( auto value : symbols[".constant"] )
	{
		if( Value::isa< BooleanConstant >( value ) )
		{
			emit( output, ((BooleanConstant*)value) );
		}
		else if( Value::isa< IntegerConstant >( value ) )
		{
			emit( output, ((IntegerConstant*)value) );
		}
		else if( Value::isa< Identifier >( value ) )
		{
		    continue;
		}
		else
		{
			assert( 0 );
		}
	}
	
	fprintf( output, "\n" );
	fprintf( output, "; functions\n" );

	for( auto value : symbols[".function"] )
	{
		assert( Value::isa< Function >( value ) );		
		emit( output, ((Function*)value) );
	}
	
	fprintf( output, "\n" );
	fprintf( output, "; derived expressions\n" );
	
	for( auto value : symbols[".derived"] )
	{
		assert( Value::isa< Derived >( value ) );		
		emit( output, ((Derived*)value) );		
	}
	
	fprintf( output, "\n" );
	fprintf( output, "; rules\n" );
	
	for( auto value : symbols[".rule"] )
	{
		assert( Value::isa< Rule >( value ) );		
		emit( output, ((Rule*)value) );		
	}

#define IND "  "
#define LF "\n"
	
	fprintf( output,
			 "define i8 @main( i32 %%args, i8** %%argv ) nounwind"
			 LF "{"
			 LF "begin:"
			 LF IND "%%mem  = alloca %%stdll.mem"
			 LF IND "call i8 @stdll.mem.new( %%stdll.mem* %%mem, i64 1000000 )"
			 LF IND "%%uset = call %%libcasm-rt.updateset* @libcasm-rt.updateset.new"
			                  "( %%stdll.mem* %%mem, i32 100 )"
			 LF IND ""
			 LF IND "call void @foo( %%libcasm-rt.updateset* %%uset )"
			 LF IND ""
			 LF IND "call i8 @libcasm-rt.updateset.dump( %%libcasm-rt.updateset* %%uset )"
			 LF IND ""
			 LF IND "%%.x = call i8* @x.location()"
			 LF IND "%%.y = call i8* @y.location()"
			 LF IND "%%.z = call i8* @z.location()"
			 LF IND ""
			 LF IND "%%x = bitcast i8* %%.x to %%libcasm-rt.Int*"
			 LF IND "%%y = bitcast i8* %%.y to %%libcasm-rt.Int*"
			 LF IND "%%z = bitcast i8* %%.z to %%libcasm-rt.Int*"
			 LF IND ""
			 LF IND "call i8 @libcasm-rt.updateset.apply( %%libcasm-rt.updateset* %%uset )"
			 LF IND ""
			 LF IND "call void @libcasm-rt.dump.Int( %%libcasm-rt.Int* %%x )"
			 LF IND "call void @libcasm-rt.dump.Int( %%libcasm-rt.Int* %%y )"
			 LF IND "call void @libcasm-rt.dump.Int( %%libcasm-rt.Int* %%z )"
			 LF IND ""
			 LF IND "call i8 @stdll.mem.del( %%stdll.mem* %%mem )"
			 LF IND "ret i8 0"
			 LF "}"
			 LF
		);
	
	return false;
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

