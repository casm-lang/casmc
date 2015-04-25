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

#include "AstToCasmIRPass.h"

/**
   @file     AstToCasmIRPass.cpp
   @author   Philipp Paulweber
   @date     2015-02-08
*/

char AstToCasmIRPass::id = 0;

static libpass::PassRegistration< AstToCasmIRPass > PASS
( "AST to CASM IR"
, "internal: translates the AST to the CASM intermeditate representation"
, 0
, 0
);


bool AstToCasmIRPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	AstWalker< AstToCasmIRPass, bool > walker( *this );
	
	walker.walk_specification( node );
	
	// TODO: PPA: implement the IR translation!!!
	
	casm_frontend_destroy();

	libcasm_ir::Value v( libcasm_ir::Value::RULE_VID );
	
	assert( 0 );

	//return true;
	return false;
}


// void AstToCasmIRPass::visit_init( UnaryNode* node )
// {
	
// }

// bool AstToCasmIRPass::visit_int_atom( IntAtom* node )
// { 
// 	printf( "%p\n", node ); 
// 	return 0; 
// }


#define VISIT printf( "%s:%i: %s: %p: %s\n", \
__FILE__, __LINE__, __FUNCTION__, node, node->to_str().c_str() )


void AstToCasmIRPass::visit_init( UnaryNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_specification( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_body_elements( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_function_def( FunctionDefNode* node, const std::vector<std::pair<T, T>>& inits )
{ VISIT;
}

void AstToCasmIRPass::visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
{ VISIT;
}
    
void AstToCasmIRPass::visit_derived_def_pre( FunctionDefNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_derived_def( FunctionDefNode* node, T expr )
{ VISIT;
}
	
void AstToCasmIRPass::visit_rule( RuleNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_statements( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_parblock( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_seqblock( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_forall_pre( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_forall_post( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_iterate( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_update( UpdateNode* node, T func, T expr )
{ VISIT;
}
	
void AstToCasmIRPass::visit_update_dumps( UpdateNode* node, T func, T expr )
{ VISIT;
}
	
void AstToCasmIRPass::visit_update_subrange( UpdateNode* node, T func, T expr)
{ VISIT;
}
	
void AstToCasmIRPass::visit_call_pre( CallNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_call_pre( CallNode* node, T expr )
{ VISIT;
}
	
void AstToCasmIRPass::visit_call( CallNode* node, std::vector< T >& args )
{ VISIT;
}
	
void AstToCasmIRPass::visit_call_post( CallNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_print( PrintNode* node, std::vector< T >& args )
{ VISIT;
}
    
void AstToCasmIRPass::visit_diedie( DiedieNode* node, T msg )
{ VISIT;
}
	
void AstToCasmIRPass::visit_impossible( AstNode* node )
{ VISIT;
}
	
void AstToCasmIRPass::visit_assert( UnaryNode* node, T expr )
{ VISIT;
}
	
void AstToCasmIRPass::visit_assure( UnaryNode* node, T expr )
{ VISIT;
}
    
void AstToCasmIRPass::visit_let( LetNode* node, T var )
{ VISIT;
}
	
void AstToCasmIRPass::visit_let_post( LetNode* node )
{ VISIT;
}
    
void AstToCasmIRPass::visit_push( PushNode* node, T expr, T atom )
{ VISIT;
}
	
void AstToCasmIRPass::visit_pop( PopNode* node )
{ VISIT;
}
    
void AstToCasmIRPass::visit_ifthenelse( IfThenElseNode* node, T cond )
{ VISIT;
}
	
void AstToCasmIRPass::visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
{ VISIT;
}
	
T AstToCasmIRPass::visit_expression( Expression* node, T lhs, T rhs )
{ VISIT;
	return 0;
}
    
T AstToCasmIRPass::visit_expression_single( Expression* node, T val )
{ VISIT;
	return 0;
}
    
T AstToCasmIRPass::visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
{ VISIT; 
	return 0;
}
    
T AstToCasmIRPass::visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	)
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_derived_function_atom( FunctionAtom* node, T expr )
{ VISIT;
	return 0;
}
    
T AstToCasmIRPass::visit_int_atom( IntAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_float_atom( FloatAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_rational_atom( RationalAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_undef_atom( UndefAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_self_atom( SelfAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_rule_atom( RuleAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_boolean_atom( BooleanAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_string_atom( StringAtom* node )
{ VISIT;
	return 0;
}
	
T AstToCasmIRPass::visit_list_atom( ListAtom* node, std::vector< T > &args )
{ VISIT; 
	return 0;
}
	
T AstToCasmIRPass::visit_number_range_atom( NumberRangeAtom* node )
{ VISIT; 
	return 0; 
}
	
T AstToCasmIRPass::visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc ) 
{ VISIT;
	return 0;
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

