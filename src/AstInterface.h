
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _ASTINTERFACE_H_
#define _ASTINTERFACE_H_

// #include ...

/**
   @file     AstInterface.h
   @class    AstInterface
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-08
*/

template<class T> class AstInterface  
{
public:

    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
	
	void visit_init( UnaryNode* node )
	{
	}
	
	void visit_specification( AstNode* node )
	{
	}
	
	void visit_body_elements( AstNode* node )
	{
	}
	
    void visit_function_def( FunctionDefNode* node, const std::vector<std::pair<T, T>>& inits )
	{
	}
	
    void visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
	{
	}

    void visit_derived_def_pre( FunctionDefNode* node )
	{
	}
	
    void visit_derived_def( FunctionDefNode* node, T expr )
	{
	}
	
    void visit_rule( RuleNode* node )
	{
	}
	
	void visit_statements( AstNode* node )
	{
	}
	
	void visit_parblock( AstNode* node )
	{
	}
	
	void visit_seqblock( AstNode* node )
	{
	}
	
	void visit_forall_pre( AstNode* node )
	{
	}
	
	void visit_forall_post( AstNode* node )
	{
	}
	
	void visit_iterate( AstNode* node )
	{
	}
	
    void visit_update( UpdateNode* node, T func, T expr )
	{
	}
	
    void visit_update_dumps( UpdateNode* node, T func, T expr )
	{
	}
	
	void visit_update_subrange( UpdateNode* node, T func, T expr)
	{
	}
	
    void visit_call_pre( CallNode* node )
	{
	}
	
    void visit_call_pre( CallNode* node, T expr )
	{
	}
	
    void visit_call( CallNode* node, std::vector< T >& args )
	{
	}
	
    void visit_call_post( CallNode* node )
	{
	}
	
    void visit_print( PrintNode* node, std::vector< T >& args )
	{
	}
    
	void visit_diedie( DiedieNode* node, T msg )
	{
	}
	
    void visit_impossible( AstNode* node )
	{
	}
	
	void visit_assert( UnaryNode* node, T expr )
	{
	}
	
    void visit_assure( UnaryNode* node, T expr )
	{
	}
    
    void visit_let( LetNode* node, T var )
	{
	}
	
    void visit_let_post( LetNode* node )
	{
	}
    
	void visit_push( PushNode* node, T expr, T atom )
	{
	}
	
    void visit_pop( PopNode* node )
	{
	}
    
	void visit_ifthenelse( IfThenElseNode* node, T cond )
	{
	}
	
	void visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
	{
	}
	
    T visit_expression( Expression* node, T lhs, T rhs )
	{
		return 0;
	}
    
	T visit_expression_single( Expression* node, T val )
	{
		return 0;
	}
    
	T visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
	{ 
		return 0;
	}
    
	T visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	)
	{
		return 0;
	}
    
	T visit_derived_function_atom( FunctionAtom* node, T expr )
	{
		return 0;
	}
    
	T visit_int_atom( IntAtom* node )
	{
		return 0;
	}
	
	T visit_float_atom( FloatAtom* node )
	{
		return 0;
	}
	
	T visit_rational_atom( RationalAtom* node )
	{
		return 0;
	}
	
	T visit_undef_atom( UndefAtom* node )
	{
		return 0;
	}
	
	T visit_self_atom( SelfAtom* node )
	{
		return 0;
	}
	
	T visit_rule_atom( RuleAtom* node )
	{
		return 0;
	}
	
	T visit_boolean_atom( BooleanAtom* node )
	{
		return 0;
	}
	
	T visit_string_atom( StringAtom* node )
	{
		return 0;
	}
	
	T visit_list_atom( ListAtom* node, std::vector< T >& args )
	{ 
		return 0;
	}
	
	T visit_number_range_atom( NumberRangeAtom* node )
	{ 
		return 0; 
	}
	
	T visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc ) 
	{
		return 0;
	}
};

#endif /* _ASTINTERFACE_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
