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

extern Driver* global_driver;

bool AstToCasmIRPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	AstWalker< AstToCasmIRPass, bool > walker( *this );

	walker.suppress_calls = true;
	walker.walk_specification( node );
	
	// TODO: PPA: implement the IR translation!!!
	
	casm_frontend_destroy();
	
	for( auto foo : (*libcasm_ir::Value::getSymbols())[ "foo" ] )
	{
		printf( "dumping 'foo':\n" );
		foo->dump();
	}
	
	assert( 0 );
    //return true;
	return false;
}

#define VISIT printf( "===--- %s:%i: %s: %p: %s\n", \
__FILE__, __LINE__, __FUNCTION__, node, node->to_str().c_str() )

#define FIXME											  \
	printf( "+++ FIXME +++: '%s:%i' in '%s'\n",			  \
			__FILE__, __LINE__, __FUNCTION__ );			  \
	fflush( stdout );									  \
	assert( 0 );



void AstToCasmIRPass::visit_specification( AstNode* node )
{
	VISIT;
	printf( "CASM specification\n" );
	//FIXME;
}

void AstToCasmIRPass::visit_init( UnaryNode* node )
{
	VISIT;
	printf( "init %s\n", global_driver->init_name.c_str() );
	//FIXME;TODO ASAP!!!
	
	// libcasm_ir::Value* ir_init =
	// 	new libcasm_ir::Value( global_driver->init_name.c_str(), 0, libcasm_ir::Value::RULE );
	
}

void AstToCasmIRPass::visit_body_elements( AstNode* node )
{
	VISIT;
	//FIXME;TODO ASAP!!!
}

void AstToCasmIRPass::visit_function_def
( FunctionDefNode* node
, const std::vector<std::pair<T, T>>& inits
)
{
	VISIT;

	string x;
	for( auto& a : node->sym->arguments_ )
	{
		x.append( a->to_str() );
	}
	
	printf( "%s, %lu: %s -> %s, \n"
			, node->sym->name.c_str()
			, node->sym->id
			, x.c_str()
			, node->sym->return_type_->to_str().c_str()
		);

	//FIXME;TODO ASAP!!!
}

void AstToCasmIRPass::visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
	string x;
	printf( "%s, %s\n"
			, node->name.c_str()
		    , x.c_str()
	    );

	FIXME;
}
    
void AstToCasmIRPass::visit_derived_def_pre( FunctionDefNode* node )
{
	VISIT;

	string x;
	for( auto& a : node->sym->arguments_ )
	{
		x.append( a->to_str() );
	}
	
	printf( "%s, %lu: %s -> %s, \n"
			, node->sym->name.c_str()
			, node->sym->id
			, x.c_str()
			, node->sym->return_type_->to_str().c_str()
		);

	FIXME;
}
	
void AstToCasmIRPass::visit_derived_def( FunctionDefNode* node, T expr )
{
	VISIT;
	FIXME;
}

void AstToCasmIRPass::visit_rule( RuleNode* node )
{
	printf( "\n\n" );
	VISIT;
	string x;
	for( auto& a : node->arguments )
	{
		x.append( a->to_str() );
	}
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	libcasm_ir::Rule* ir_rule = new libcasm_ir::Rule( node->name.c_str() );
    assert( ir_rule );
    ast2casmir[ node ]        = ir_rule;
	ast2parent[ node->child_ ] = node;
}

void AstToCasmIRPass::visit_parblock( UnaryNode* node )
{
	VISIT;
	printf( "{ }\n" );
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	libcasm_ir::ParallelBlock* ir_scope = new libcasm_ir::ParallelBlock();
	assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
	ast2parent[ node->child_ ] = node;
	
	libcasm_ir::Rule* ir_rule =	lookupParent< libcasm_ir::Rule >( node );
	
	if( ir_rule )
	{
		ir_rule->setContext( ir_scope );
	}
	else
	{
		libcasm_ir::ExecutionSemanticsBlock* ir_parent_scope =
			lookupParent< libcasm_ir::ExecutionSemanticsBlock >( node );
		
		assert( ir_parent_scope );
		ir_parent_scope->add( ir_scope );
	}
}

void AstToCasmIRPass::visit_seqblock( UnaryNode* node )
{	
	VISIT;
	printf( "{| |}\n" );
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	libcasm_ir::SequentialBlock* ir_scope = new libcasm_ir::SequentialBlock();
	assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
	ast2parent[ node->child_ ] = node;
	
	libcasm_ir::ExecutionSemanticsBlock* ir_parent_scope =
		lookupParent< libcasm_ir::ExecutionSemanticsBlock >( node );
	
	assert( ir_parent_scope );
	ir_parent_scope->add( ir_scope );
}

void AstToCasmIRPass::visit_statements( AstListNode* node )
{
    VISIT;
    printf( "...\n" );
	for( AstNode *s : node->nodes )
	{
		printf( "%p -> %p\n", node, s );
	}
	
	assert( node );
	
	auto parent = ast2parent.find( node );
	assert( parent != ast2parent.end() );
	
	for( AstNode *s : node->nodes )
	{
		ast2parent[ s ] = parent->second;
	}
}
	
void AstToCasmIRPass::visit_forall_pre( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_forall_post( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_iterate( AstNode* node )
{
	VISIT;
	FIXME;
}

void AstToCasmIRPass::visit_update( UpdateNode* node, T func, T expr )
{
	VISIT;
    printf( "%p -> %p\n", node, node->func );
	printf( "%p -> %p\n", node, node->expr_ );
	
	libcasm_ir::ExecutionSemanticsBlock* ir_scope =
		lookupParent< libcasm_ir::ExecutionSemanticsBlock >( node );
	
	assert( ir_scope );
	
	libcasm_ir::TrivialStatement* ir_stmt = new libcasm_ir::TrivialStatement( ir_scope );
	assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
	ast2parent[ node->func ]   = node;
	ast2parent[ node->expr_ ]  = node;
	
	libcasm_ir::Value* ir_lhs = lookup< libcasm_ir::Value >( node->func  );
	libcasm_ir::Value* ir_rhs = lookup< libcasm_ir::Value >( node->expr_ );
	
	assert( ir_lhs );
	assert( ir_rhs );
	
	ir_stmt->add( new libcasm_ir::UpdateInstruction( ir_lhs, ir_rhs ) );
}

void AstToCasmIRPass::visit_update_dumps( UpdateNode* node, T func, T expr )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_update_subrange( UpdateNode* node, T func, T expr)
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_call_pre( CallNode* node )
{
    //VISIT;
}
	
void AstToCasmIRPass::visit_call_pre( CallNode* node, T expr )
{
    //VISIT;
}

void AstToCasmIRPass::visit_call( CallNode* node, std::vector< T >& args )
{
	VISIT;

	printf( "call: %s\n", node->rule_name.c_str() );

	FIXME;
}

void AstToCasmIRPass::visit_call_post( CallNode* node )
{
	//VISIT;
}

void AstToCasmIRPass::visit_print( PrintNode* node, std::vector< T >& args )
{
	VISIT;
	string x;
// 	for( auto& a : node->atoms )
// 	{
// //		x.append( a->to_str() );
// 	}
	
	printf( "print: %s\n"
		    , x.c_str()
	    );
	
	FIXME;
}
    
void AstToCasmIRPass::visit_diedie( DiedieNode* node, T msg )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_impossible( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_assert( UnaryNode* node, T expr )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_assure( UnaryNode* node, T expr )
{
	VISIT;
	FIXME;
}
    
void AstToCasmIRPass::visit_let( LetNode* node, T var )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_let_post( LetNode* node )
{
	VISIT;
	FIXME;
}
    
void AstToCasmIRPass::visit_push( PushNode* node, T expr, T atom )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_pop( PopNode* node )
{
	VISIT;
	FIXME;
}
    
void AstToCasmIRPass::visit_ifthenelse( IfThenElseNode* node, T cond )
{
	VISIT;
	FIXME;
}
	
void AstToCasmIRPass::visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
{
	VISIT;
	FIXME;
}

T AstToCasmIRPass::visit_expression( Expression* node, T lhs, T rhs )
{
	VISIT;
	printf( "%s, %p, %p\n", operator_to_str( node->op ).c_str(), node->left_, node->right_ );
	
	libcasm_ir::Value* ir_lhs = lookup< libcasm_ir::Value >( node->left_  );
	libcasm_ir::Value* ir_rhs = lookup< libcasm_ir::Value >( node->right_ );

	assert( ir_lhs );
	assert( ir_rhs );
	
	libcasm_ir::Value* ir_expr = 0;

	switch( node->op )
	{
	    case ExpressionOperation::ADD:
			ir_expr = new libcasm_ir::AddInstruction( ir_lhs, ir_rhs );
			break;
		
	    default:
			assert( 0 && "unsupported expression operation" );
			FIXME;
	}
	
	assert( ir_expr );
	ast2casmir[ node ] = ir_expr;
	
	return 0;
}
    
T AstToCasmIRPass::visit_expression_single( Expression* node, T val )
{
	VISIT;
	FIXME;
	return 0;
}
    
T AstToCasmIRPass::visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
	printf( "%s, %p", node->name.c_str(), node );
	
    for( auto a : *(node->arguments) )
	{
		printf( ", %p", a );
	}
	printf( "\n" );
	
	libcasm_ir::LocationInstruction* ir_loc
		= new libcasm_ir::LocationInstruction(
			new libcasm_ir::Value( node->name.c_str(), 0, libcasm_ir::Value::RULE )
                                                          // PPA: FIXME: HACK: TODO:
			);
	
	assert( ir_loc );
    for( auto a : *(node->arguments) )
	{
		libcasm_ir::Value* instr = lookup< libcasm_ir::Value >( a );
	    ir_loc->add( instr );		
	}
    
	libcasm_ir::LookupInstruction* ir_lup
		= new libcasm_ir::LookupInstruction( ir_loc );
	
	assert( ir_lup );
    ast2casmir[ node ] = ir_lup;
	
	return 0;
}
    
T AstToCasmIRPass::visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	)
{
	VISIT;
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_derived_function_atom( FunctionAtom* node, T expr )
{
	VISIT;
	string x;
	printf( "derived: %s, %s\n"
			, node->name.c_str()
		    , x.c_str()
	    );
	FIXME;
	return 0;
}
    
T AstToCasmIRPass::visit_int_atom( IntAtom* node )
{
	VISIT;
	printf( "%lu\n", node->val_	);
	
	libcasm_ir::IntegerConstant* ir_const
		= new libcasm_ir::IntegerConstant( ( libcasm_ir::Type::Integer )node->val_ );
	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}
	
T AstToCasmIRPass::visit_float_atom( FloatAtom* node )
{
	VISIT;
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_rational_atom( RationalAtom* node )
{
	VISIT;
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_undef_atom( UndefAtom* node )
{
	VISIT;
	printf( "undef\n" );
	
	libcasm_ir::Value* ir_const	= new libcasm_ir::UndefConstant();
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}
	
T AstToCasmIRPass::visit_self_atom( SelfAtom* node )
{
	VISIT;
	printf( "self\n" );
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_rule_atom( RuleAtom* node )
{
	VISIT;
	printf( "rule: @%s\n", node->name.c_str() );
	FIXME;
	return 0;
}

T AstToCasmIRPass::visit_boolean_atom( BooleanAtom* node )
{
	VISIT;
	printf( "%u\n", node->value );
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_string_atom( StringAtom* node )
{
	VISIT;
	printf( "%s\n", node->string.c_str() );
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_list_atom( ListAtom* node, std::vector< T > &args )
{
	VISIT; 
	FIXME;
	return 0;
}
	
T AstToCasmIRPass::visit_number_range_atom( NumberRangeAtom* node )
{
	VISIT; 
	FIXME;
	return 0; 
}
	
T AstToCasmIRPass::visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc ) 
{
	VISIT;
	FIXME;
	return 0;
}



template<class C>
C* AstToCasmIRPass::lookupParent( AstNode* node )
{
	//printf( "%s: ??? %p\n", __FUNCTION__, node );
		
	auto result = ast2parent.find( node );
	if( result != ast2parent.end() )
	{
		//printf( "%s: %p, %p\n", __FUNCTION__, result->first, result->second );
		return lookup< C >( result->second );
	}
		
	return 0;
}

template<class C>
C* AstToCasmIRPass::lookup( AstNode* node )
{
	printf( "%s: %p\n", __FUNCTION__, node );
		
	auto result = ast2casmir.find( node );
	if( result != ast2casmir.end() )
	{
		printf( "%s: %p\n", __FUNCTION__, result->second );
		if( libcasm_ir::Value::isa< C >( result->second ) )
		{
			return static_cast< C* >( result->second );
		}
		else
		{
			assert( 0 );
		}
	}
		
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

