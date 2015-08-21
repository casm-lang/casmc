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

static libcasm_ir::Type* getType( Type* type )
{
	assert( type && "not initialized type used" );
	
	switch( type->t )
	{
	    case TypeType::INT:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::INTEGER );
	    case TypeType::BOOLEAN:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::BOOLEAN );
	    default:
			assert( 0 && "not implemented function atom identifier type" );
			return 0;
	}
}

bool AstToCasmIRPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	AstWalker< AstToCasmIRPass, bool > walker( *this );

	walker.suppress_calls = true;
	walker.walk_specification( node );
	
	// TODO: PPA: implement the IR translation!!!
	
	casm_frontend_destroy();

	
	std::string input = "";

	while( input.compare( "q" ) != 0 )
	{
		std::getline( cin, input );

		if( input.compare( "s" ) == 0 )
		{
			assert( 0 );
		}
		else
		{
			for( auto r : (*libcasm_ir::Value::getSymbols())[ input.c_str() ] )
			{
				printf( "dumping '%s':\n", input.c_str() );
				r->dump();
			}
		}
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

	
	libcasm_ir::Type* ftype = getType( node->sym->return_type_ );
	for( auto argument : node->sym->arguments_ )
	{
		ftype->addParameter( getType( argument ) );
	}
		
	libcasm_ir::Function* ir_function =
		new libcasm_ir::Function( node->sym->name.c_str(), ftype );
	assert( ir_function );
}

    
void AstToCasmIRPass::visit_derived_def_pre( FunctionDefNode* node )
{
	VISIT;
}
	
void AstToCasmIRPass::visit_derived_def( FunctionDefNode* node, T expr )
{
	VISIT;
	string x;
	for( auto& a : node->sym->arguments_ )
	{
		x.append( a->to_str() );
	}
	printf( "%s, %lu: %s -> %s, \n", node->sym->name.c_str(), node->sym->id, x.c_str(), node->sym->return_type_->to_str().c_str() );
	
	assert( node->sym->type == Symbol::SymbolType::DERIVED );
	
	libcasm_ir::Value* ir_expr = lookup< libcasm_ir::Value >( node->sym->derived  );
	assert( ir_expr );
	
	libcasm_ir::TrivialStatement* ir_stmt = new libcasm_ir::TrivialStatement();
	assert( ir_stmt );
	ir_stmt->add( ir_expr );

	
	libcasm_ir::Type* ftype = getType( node->sym->return_type_ );
	for( auto argument : node->sym->arguments_ )
	{
		ftype->addParameter( getType( argument ) );
	}
	
	libcasm_ir::Derived* ir_derived =
		new libcasm_ir::Derived( node->sym->name.c_str(), ftype );
	assert( ir_derived );
	ir_derived->setContext( ir_stmt );
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
	
	libcasm_ir::Value* parent = lookupParent< libcasm_ir::Value >( node );
	assert( parent );
	
	if( libcasm_ir::Value::isa< libcasm_ir::Rule >( parent ) )
	{
		((libcasm_ir::Rule*)parent)->setContext( ir_scope );
	}
	else if( libcasm_ir::Value::isa< libcasm_ir::ExecutionSemanticsBlock >( parent ) )
	{
		((libcasm_ir::ExecutionSemanticsBlock*)parent)->add( ir_scope );
	}
	else if( libcasm_ir::Value::isa< libcasm_ir::Statement >( parent ) )
	{
		((libcasm_ir::Statement*)parent)->getScope()->add( ir_scope );
	}
	else
	{
		assert( 0 );
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

	libcasm_ir::Value* parent = lookupParent< libcasm_ir::Value >( node );
	assert( parent );
	
    if( libcasm_ir::Value::isa< libcasm_ir::ExecutionSemanticsBlock >( parent ) )
	{
		((libcasm_ir::ExecutionSemanticsBlock*)parent)->add( ir_scope );
	}
	else if( libcasm_ir::Value::isa< libcasm_ir::Statement >( parent ) )
	{
		((libcasm_ir::Statement*)parent)->getScope()->add( ir_scope );
	}
	else
	{
		assert( 0 );
	}
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
	
	libcasm_ir::ExecutionSemanticsBlock* ir_scope =
		lookupParent< libcasm_ir::ExecutionSemanticsBlock >( node );
	assert( ir_scope );
	
	libcasm_ir::TrivialStatement* ir_stmt = new libcasm_ir::TrivialStatement( ir_scope );
	assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
	// ast2parent[ node->func ]   = node;
	// ast2parent[ node->expr_ ]  = node;
    
	libcasm_ir::PrintInstruction* ir_print = new libcasm_ir::PrintInstruction();
    assert( ir_print );
	for( auto a : node->atoms )
	{
		libcasm_ir::Value* instr = lookup< libcasm_ir::Value >( a );
		ir_print->add( instr );
	}
	ir_stmt->add( ir_print );
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
	
	ast2parent[ node->expr ] = node;
	ast2parent[ node->stmt ] = node;
	
	libcasm_ir::Value* ir_expr = lookup< libcasm_ir::Value >( node->expr );
	assert( ir_expr );
	
	libcasm_ir::Value* ir_ident =
		libcasm_ir::Identifier::create( getType( &node->type_ ), node->identifier.c_str() );
	assert( ir_ident );
	
	libcasm_ir::Value* ir_let = new libcasm_ir::LetInstruction( ir_ident, ir_expr );
	assert( ir_let );
    
	libcasm_ir::ExecutionSemanticsBlock* ir_scope = 0;
	libcasm_ir::Value* parent = lookupParent< libcasm_ir::Value >( node );
	assert( parent );
	if( libcasm_ir::Value::isa< libcasm_ir::ExecutionSemanticsBlock >( parent ) )
	{
		ir_scope = (libcasm_ir::ExecutionSemanticsBlock*)parent;
		if( ir_scope->isParallel() )
		{
			libcasm_ir::ExecutionSemanticsBlock* ir_scope_top = ir_scope;
		    ir_scope = new libcasm_ir::SequentialBlock();
			assert( ir_scope );
			ir_scope->setParent( ir_scope_top );
			ir_scope_top->add( ir_scope );
		}		
	}
	else if( libcasm_ir::Value::isa< libcasm_ir::Statement >( parent ) )
	{
		ir_scope = ((libcasm_ir::Statement*)parent)->getScope();
		printf( "up for %p\n", parent );
	}
	else
	{
		assert( 0 );
	}
	
	libcasm_ir::TrivialStatement* ir_stmt = new libcasm_ir::TrivialStatement( ir_scope );
	assert( ir_stmt );
	ast2casmir[ node ] = ir_stmt;
	
	//ir_scope->add( ir_stmt );
	ir_stmt->add( ir_let );
}

void AstToCasmIRPass::visit_let_post( LetNode* node )
{
	VISIT;	
	libcasm_ir::Identifier::forgetSymbol( node->identifier.c_str() );
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
		case ExpressionOperation::SUB:
			ir_expr = new libcasm_ir::SubInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::MUL:
			ir_expr = new libcasm_ir::MulInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::DIV:
			ir_expr = new libcasm_ir::DivInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::RAT_DIV:
			ir_expr = new libcasm_ir::RivInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::MOD:
			ir_expr = new libcasm_ir::ModInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::EQ:
			ir_expr = new libcasm_ir::EquInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::NEQ:
			ir_expr = new libcasm_ir::NeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::LESSER:
			ir_expr = new libcasm_ir::LesInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::LESSEREQ:
			ir_expr = new libcasm_ir::LeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::GREATER:
			ir_expr = new libcasm_ir::GreInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::GREATEREQ:
			ir_expr = new libcasm_ir::GeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::OR:
			ir_expr = new libcasm_ir::OrInstruction(  ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::XOR:
			ir_expr = new libcasm_ir::XorInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::AND:
			ir_expr = new libcasm_ir::AndInstruction( ir_lhs, ir_rhs );
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
	printf( "%s, %p\n", operator_to_str( node->op ).c_str(), node->left_ );
	
	libcasm_ir::Value* ir_lhs = lookup< libcasm_ir::Value >( node->left_  );
	assert( ir_lhs );
	
	libcasm_ir::Value* ir_expr = 0;
	switch( node->op )
	{
		case ExpressionOperation::NOT:
			ir_expr = new libcasm_ir::NotInstruction( ir_lhs );
			break;
	    default:
			assert( 0 && "unsupported expression operation" );
			FIXME;
	}
	
	assert( ir_expr );
	ast2casmir[ node ] = ir_expr;
	return 0;
}
    
T AstToCasmIRPass::visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
	printf( "%s, %p", node->name.c_str(), node );
	if( node->arguments )
	{
		for( auto a : *(node->arguments) )
		{
			printf( ", %p", a );
		}
	}
	printf( "\n" );
	
	if( node->symbol_type == FunctionAtom::SymbolType::PARAMETER )
	{
		libcasm_ir::Value* ir_ident = libcasm_ir::Identifier::create( getType( &node->type_ ), node->name.c_str() );
		assert( ir_ident );		
		ast2casmir[ node ] = ir_ident;
		return 0;
	}
	
	libcasm_ir::Type* ty_ident = 0;
	switch( node->symbol_type )
	{
	    case FunctionAtom::SymbolType::FUNCTION:
		{
			assert( node->symbol );
			ty_ident = getType( node->symbol->return_type_ );
			for( auto argument : node->symbol->arguments_ )
			{
				ty_ident->addParameter( getType( argument ) );
			}
			break;
		}
	    default:
			assert( 0 && "not implemented function atom identifier symbol type" );
	}
	
	libcasm_ir::Value* ir_ident = libcasm_ir::Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
	
	libcasm_ir::LocationInstruction* ir_loc	= new libcasm_ir::LocationInstruction( ir_ident );
    assert( ir_loc );
	if( node->arguments )
	{
		for( auto a : *(node->arguments) )
		{
			libcasm_ir::Value* instr = lookup< libcasm_ir::Value >( a );
			ir_loc->add( instr );		
		}
    }
	
	libcasm_ir::LookupInstruction* ir_lup = new libcasm_ir::LookupInstruction( ir_loc );
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

void AstToCasmIRPass::visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
}

T AstToCasmIRPass::visit_derived_function_atom( FunctionAtom* node, T expr )
{
	VISIT;
	string x;
	printf( "derived: %s, %s\n"
			, node->name.c_str()
		    , x.c_str()
	    );

	assert( node->symbol );
	assert( node->symbol_type == FunctionAtom::SymbolType::DERIVED );
	
	libcasm_ir::Type* ty_ident = getType( node->symbol->return_type_ );
	for( auto argument : node->symbol->arguments_ )
	{
		ty_ident->addParameter( getType( argument ) );
	}
	
	libcasm_ir::Value* ir_ident = libcasm_ir::Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
	
	libcasm_ir::CallInstruction* ir_call = new libcasm_ir::CallInstruction( ir_ident );
    assert( ir_call );
    ast2casmir[ node ] = ir_call;
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            libcasm_ir::Value* instr = lookup< libcasm_ir::Value >( a );
            ir_call->add( instr );		
        }
    }
	
	return 0;
}

T AstToCasmIRPass::visit_boolean_atom( BooleanAtom* node )
{
	VISIT;
	printf( "%u\n", node->value );
	
	libcasm_ir::BooleanConstant* ir_const
		= new libcasm_ir::BooleanConstant( ( libcasm_ir::Type::Boolean )node->value );
	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;

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
	
T AstToCasmIRPass::visit_string_atom( StringAtom* node )
{
	VISIT;
	printf( "%s\n", node->string.c_str() );
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

	libcasm_ir::Value* ir_const	= new libcasm_ir::SelfConstant();
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}
	
T AstToCasmIRPass::visit_rule_atom( RuleAtom* node )
{
	VISIT;
	printf( "rule: @%s\n", node->name.c_str() );
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
	//printf( "%s: %p\n", __FUNCTION__, node );
		
	auto result = ast2casmir.find( node );
	if( result != ast2casmir.end() )
	{
		//printf( "%s: %p\n", __FUNCTION__, result->second );
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

