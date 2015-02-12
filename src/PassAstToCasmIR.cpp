
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#include "PassAstToCasmIR.h"

/**
   @file     PassAstToCasmIR.cpp
   @author   Philipp Paulweber
   @date     2015-02-08
*/

PassAstToCasmIR::PassAstToCasmIR
( Driver& driver 
)
: ast_driver( driver )
{
}

// void PassAstToCasmIR::visit_init( UnaryNode* node )
// {
	
// }

// bool PassAstToCasmIR::visit_int_atom( IntAtom* node )
// { 
// 	printf( "%p\n", node ); 
// 	return 0; 
// }


#define VISIT printf( "%s:%i: %p: %s\n", __FILE__, __LINE__, node, node->to_str().c_str() )


void PassAstToCasmIR::visit_init( UnaryNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_specification( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_body_elements( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_function_def( FunctionDefNode* node, const std::vector<std::pair<T, T>>& inits )
{ VISIT;
}

void PassAstToCasmIR::visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
{ VISIT;
}
    
void PassAstToCasmIR::visit_derived_def_pre( FunctionDefNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_derived_def( FunctionDefNode* node, T expr )
{ VISIT;
}
	
void PassAstToCasmIR::visit_rule( RuleNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_statements( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_parblock( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_seqblock( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_forall_pre( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_forall_post( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_iterate( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_update( UpdateNode* node, T func, T expr )
{ VISIT;
}
	
void PassAstToCasmIR::visit_update_dumps( UpdateNode* node, T func, T expr )
{ VISIT;
}
	
void PassAstToCasmIR::visit_update_subrange( UpdateNode* node, T func, T expr)
{ VISIT;
}
	
void PassAstToCasmIR::visit_call_pre( CallNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_call_pre( CallNode* node, T expr )
{ VISIT;
}
	
void PassAstToCasmIR::visit_call( CallNode* node, std::vector< T >& args )
{ VISIT;
}
	
void PassAstToCasmIR::visit_call_post( CallNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_print( PrintNode* node, std::vector< T >& args )
{ VISIT;
}
    
void PassAstToCasmIR::visit_diedie( DiedieNode* node, T msg )
{ VISIT;
}
	
void PassAstToCasmIR::visit_impossible( AstNode* node )
{ VISIT;
}
	
void PassAstToCasmIR::visit_assert( UnaryNode* node, T expr )
{ VISIT;
}
	
void PassAstToCasmIR::visit_assure( UnaryNode* node, T expr )
{ VISIT;
}
    
void PassAstToCasmIR::visit_let( LetNode* node, T var )
{ VISIT;
}
	
void PassAstToCasmIR::visit_let_post( LetNode* node )
{ VISIT;
}
    
void PassAstToCasmIR::visit_push( PushNode* node, T expr, T atom )
{ VISIT;
}
	
void PassAstToCasmIR::visit_pop( PopNode* node )
{ VISIT;
}
    
void PassAstToCasmIR::visit_ifthenelse( IfThenElseNode* node, T cond )
{ VISIT;
}
	
void PassAstToCasmIR::visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
{ VISIT;
}
	
T PassAstToCasmIR::visit_expression( Expression* node, T lhs, T rhs )
{ VISIT;
	return 0;
}
    
T PassAstToCasmIR::visit_expression_single( Expression* node, T val )
{ VISIT;
	return 0;
}
    
T PassAstToCasmIR::visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
{ VISIT; 
	return 0;
}
    
T PassAstToCasmIR::visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	)
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_derived_function_atom( FunctionAtom* node, T expr )
{ VISIT;
	return 0;
}
    
T PassAstToCasmIR::visit_int_atom( IntAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_float_atom( FloatAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_rational_atom( RationalAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_undef_atom( UndefAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_self_atom( SelfAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_rule_atom( RuleAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_boolean_atom( BooleanAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_string_atom( StringAtom* node )
{ VISIT;
	return 0;
}
	
T PassAstToCasmIR::visit_list_atom( ListAtom* node, std::vector< T > &args )
{ VISIT; 
	return 0;
}
	
T PassAstToCasmIR::visit_number_range_atom( NumberRangeAtom* node )
{ VISIT; 
	return 0; 
}
	
T PassAstToCasmIR::visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc ) 
{ VISIT;
	return 0;
}


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/

