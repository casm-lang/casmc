
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'casmc' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _PASSRESULT_H_
#define _PASSRESULT_H_

#include "Type.h"

/**
   @file     PassResult.h
   @class    PassResult
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-14
*/

class PassResult  
{
// public:
// 	typedef Type::IruleVector RuleVectorType;

// private:
// 	static Type::IruleVector& registeredRules(void)
// 	{
// 		static Type::IruleVector rules;
// 		return rules;
// 	}

// public:
// 	static void registerRule(irule* passRule)
// 	{
// 		assert( passRule != 0 && "invalid pass rule object pointer" );
// 		registeredRules().push_back( passRule );
// 	}

// 	static Type::IruleVector& getRegisteredRules(void)
// 	{
// 		return registeredRules();
// 	}

private:
	// irule* rule;
	
	// uint64_t constCounter;

	// uint64_t variableCounter;
	
	PassId2Ptr results;
	
	PassId2u64 changes;
	
	// PassIdUInt64PairVector trace;

	// stringstream traceStream;
	
public:
    PassResult( ) //irule* passRule) 
		// : rule( passRule ),
		//   constCounter( 0 ),
		//   variableCounter( 0 )
	{
		// assert( passRule != 0 && "invalid pass rule object pointer" );
		// DEBUG("created new pass phase '" << passRule->name << "'\n");
		
		// rule->body_copy = ast_duplicate( rule->body );		
		// DEBUG("duplicated ast rule body!\n");
		
		// ast_label_rule(rule);
		// DEBUG("labeled the ast\n");
	}
	
	~PassResult()
	{
		// ast_label_rule(rule);
		// DEBUG("labeled the ast (END)\n");

		// results.clear();
		// changes.clear();
		// trace.clear();
	}
	
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
 
	// uint64_t getConstCounter(void) const
	// {
	// 	return constCounter;
	// }	

	// void incrementConstCounter(void)
	// {
	// 	constCounter++;
	// }	
	
	
	// uint64_t getVariableCounter(void) const
	// {
	// 	return variableCounter;
	// }	

	// void incrementVariableCounter(void)
	// {
	// 	variableCounter++;
	// }
	
	
	
	// bool isRedundant(PassId id)
	// {
	// 	for( int64_t sr = trace.size() - 1; sr >= 0; sr-- )
	// 	{
	// 		// AST has changed in another previous Pass 
	// 		if( trace[ sr ].second != 0 ) return false;

	// 		// this Pass was already perfomed!                     
	// 		if( trace[ sr ].first == id )
	// 		{
	// 			return true;
	// 		}
	// 	}
		
	// 	return false;
	// }
	

	// ast* getAST(void)
	// {
	// 	return rule->body;
	// }

	// void setAST(ast* oldnode, ast* newnode)
	// {
	// 	assert(getAST() == oldnode);
		
	// 	//AstUtility::dealloc(rule->body);
		
	// 	rule->body = newnode;
		
	// 	// but rule->body is only a copy, we need to modify the original AST as well
	// 	// there are 2 variants possible
			
	// 	ast* element = rule->ast_position;

	// 	while( element != NULL && element->right != oldnode )
	// 	{
	// 		element = element->right;
	// 	}

	// 	assert( element != NULL );
                
	// 	element->right = newnode;
	// }
	
	// const char* getRuleName(void) const
	// {
	// 	return rule->name;
	// }
	
	// irule* getIrule(void) const
	// {
	// 	return rule;
	// }	
	
	template<class PassName>
	void* getResult(void)
	{
		//DEBUG( __FUNCTION__ << ": " << &PassName::id << " = " << results[ &PassName::id ] 
		//<< "\n" );
		
		return results[ &PassName::id ];
	}
	
	template<class PassName>
	void setResult(void* passResult)
	{
		//DEBUG( __FUNCTION__ << ": " << &PassName::id << " = " << passResult << "\n" );
		
		results[ &PassName::id ] = passResult;
	}
	
	PassId2Ptr& getResults(void)
	{
		return results;
	}
	
	void printAllResults(void)
	{
		// DEBUG( __FUNCTION__ << ": " << results.size() << "\n" );
		
		// for( auto& r : results )
		// {
		// 	DEBUG( __FUNCTION__ << ": " << r.first << " = " << r.second << "\n" );
		// }
	}
	
	uint64_t getChanges(PassId id)
	{
		return changes[ id ];
	}
	
	template<class PassName>
	void setChanges(uint64_t passChanges)
	{
		changes[ &PassName::id ] = passChanges;
		
		//trace.push_back( PassIdUInt64Pair( &PassName::id, passChanges ) );
		
		if( passChanges > 0 )
		{
			//traceStream << "." << PassRegistry::getPassInfo( &PassName::id ).getPassArgument();
		} 
	}
	
	void setChanges(PassId id, uint64_t passChanges)
	{
		changes[ id ] = passChanges;
		
		//trace.push_back( PassIdUInt64Pair( id, passChanges ) );
	}
	
	// string getTrace(void) const
	// {
	// 	return traceStream.str();
	// }	
	
	friend ostream& operator<<(ostream& os, PassResult& pr)
	{
		return os; // << pr.getRuleName() << ": ";
	}	
};

#endif /* _PASSRESULT_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
