#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

// I'm not sure if next line works with other than GNU compiler
#include <cxxabi.h>

#include "TreeNode.hpp"

using namespace std;

namespace ensc251 {

		class Token;

		unsigned int tracker = 0; // tracker index
		std::vector<Token*> tokenObjectPs; // objects for abstract syntax tree

		static std::string empty = "";

		char *demangle(const char *typeName) {
			int status;
			return abi::__cxa_demangle(typeName,0,0,&status);
		}

        class Token : public TreeNode
        {
        	// The Base Token Class
        public:

        virtual const std::string& get_string_value() { return empty; };

		friend std::ostream& operator <<(std::ostream& outs, const Token& token){
			token.print(outs);
        	return outs;
		}

		virtual Token* process_punctuator(const std::string &puncValue) { return nullptr; }
		virtual Token* process_unary() { return nullptr; }
		virtual Token* process_primary_exp() { return nullptr; }
    	virtual Token* advance_past_shift_operator() { return nullptr; }
    	virtual Token* advance_past_assignment_operator() { return nullptr; }

    	virtual Token* process_id() { return nullptr; }

    	Token* process_token()
    	{
			tracker++;
			// should we check to see if we are at '}'
    		if (tracker < tokenObjectPs.size()){
    			return this;
    		}
    		else
    			// our grammar must end in '}'. If *this is the end
    			//  then it is a premature end which we will report now.
    			return nullptr;
    	}

       void print(std::ostream& str) const { str << demangle(typeid(*this).name()) << ": "; }
       virtual void print_node_values() const { }; // = 0; // make pure??
 	   virtual ~Token() {}

		};

        // we should be able to re-order the functions to get of most or all of these forward declarations
  	  Token* recursive_parser();
        std::vector<Token*> decl_list();
        std::vector<Token*> stat_list();
        Token* stat();
        Token* exp_stat();
        Token* exp();
        Token* assignment_exp();
        Token* ternary_exp();
        Token* unary_exp();
        Token* shift_exp();
        Token* additive_exp(); // missing
        Token* div_exp(); // missing
        Token* unary_exp();
        Token* postfix_exp();

        Token* recursive_parser()
        {
        	if (tokenObjectPs.size())
        		return tokenObjectPs[tracker]->process_punctuator("{");
        	else
        		return nullptr;
        }

        std::vector<Token*> decl_list()
        {
        	std::vector<Token*> all_declarations;

			// ***** Complete this function and add code elsewhere as needed *****

        	return all_declarations;
        }

        std::vector<Token*> stat_list()
        {
        	std::vector<Token*> all_stats;
        	while(auto subTree = stat()) {
        		all_stats.push_back(subTree);
        		if(tracker < tokenObjectPs.size() && tokenObjectPs[tracker]->get_string_value() == "}")
        			return all_stats;
        	}
        	return {};
        }

        Token* stat()
        {
        	//int old_tracker = tracker;
        	if(Token* subTree = exp_stat())
        		return subTree;
			// ***** Complete this function

        	return nullptr;
        }

        Token* exp_stat() // expression statement
        {
        	if(Token* subTree = exp())
        	{
        		if (tokenObjectPs[tracker]->process_punctuator(";"))
        			return subTree;
        	}
        	return nullptr;
        }

        Token* exp() // expression
        {
        	if(Token* subTree = assignment_exp())
        	{
    			// ***** Complete this function
				return subTree;
				// ***** Complete this function
        	}
			return nullptr;
        }

        Token* assignment_exp()
        {
        	int old_tracker = tracker;

        	// production: unary_exp assignment_operator assignment_exp
        	Token* subTree = unary_exp();
        	if(subTree)
        	{
        		if(Token* tObj = tokenObjectPs[tracker]->advance_past_assignment_operator())
        		{ // the pattern here
        			tObj->add_childP(subTree);
        			subTree = assignment_exp();
        			if(subTree)
        			{
        				tObj->add_childP(subTree);
        				return tObj;
        			}
        			else
        				// we have assignment operator, but not valid assignment expression,
        				//	so there must be an error and it should be reported.
        				return nullptr;
        		}
        		// no assignment operator, so this is not the first production

        		// start production:  ternary_exp
        		if(tokenObjectPs[tracker]->get_string_value() == ";")
        			// we have already built a unary expression subtree, which satisfies the ternary_exp production.
        			return subTree;
        	}

        	// production:  ternary_exp
        	tracker = old_tracker;
        	subTree = ternary_exp();
        	if(subTree)
        		return subTree;
        	else
        		return nullptr;
        }

        Token* ternary_exp()
        {
        	int old_tracker = tracker;
        	Token* subTree;

			// ***** Complete this function

        	tracker = old_tracker;
        	subTree = shift_exp();
        	if(subTree)
        		return subTree;
        	else
        		return nullptr;
        }

		// ***** Add more functions around here somewhere *****

        Token* shift_exp()
        // should be: additive_exp {shift_operator additive_exp}
        // implemented: additive_exp {shift_operator unary_exp}
        {
        	// unary_exp substituted for additive_exp

        	Token* holdAeTreeP = nullptr;
        	Token* aeTreeP = nullptr;
        	Token* lowerNodeP = nullptr;
        	Token* upperNodeP = nullptr;
        	if ((aeTreeP = unary_exp())) { // unary_exp substituted for additive_exp

        		// unary_exp substituted for additive_exp
        		while ((lowerNodeP = tokenObjectPs[tracker]->advance_past_shift_operator()) && (holdAeTreeP = unary_exp())) {
        			if (upperNodeP)
        				upperNodeP->add_childP(lowerNodeP); // completes upper node children

        			lowerNodeP->add_childP(aeTreeP); // lower Node will need one more Child.
        			upperNodeP = lowerNodeP;
        			aeTreeP = holdAeTreeP;
        		}
        		if (upperNodeP) {
        			upperNodeP->add_childP(aeTreeP); // completes current upper node
        			return upperNodeP;
        		}
        	}
        	return aeTreeP;
        }

        Token* unary_exp()
        {
        	int old_tracker = tracker;

        	Token* tObjP;

        	if(!(tObjP = tokenObjectPs[tracker]->process_unary()))
        	{
        		tracker = old_tracker;
        		tObjP = postfix_exp();
        	}
        	return tObjP;
        }

        Token* postfix_exp()
        {
        	auto subTree = tokenObjectPs[tracker]->process_primary_exp();
			// ***** Complete this function
        	return subTree;
        }

        class StringBasedToken:public Token {
			// subclasses: id, type_spec, assignment_operator, shift_operator, additive_operator, mul_operator, unary_operator, string, Const
		protected:
			std::string stringValue;

		public:
			StringBasedToken(const std::string &aStringValue): stringValue (aStringValue) { };
			void print(std::ostream& str) const { Token::print(str); str << ": " << stringValue; }
			const std::string& get_string_value() { return stringValue; }
			void print_node_values() const { cout << stringValue << " "; };
        };

        class punctuator:public StringBasedToken {
        	// e.g., ';', '*', '{', '}', ...
        public:
        	punctuator(const std::string &puncValue): StringBasedToken(puncValue){ };
        	void traverse_node(action_func f) const
			{
        		char punctChar = stringValue[0];

        		switch (punctChar)
        		{
        		case '{':
        			cout << punctChar << endl;
        			for (auto child: childPVector) {
        				child->traverse_node(f);
        				cout << ";" << endl;
        			}
        			cout << "}" << endl;
        			break;

        		case '*':
        			in_order_traverse_node(f);
        			break;

        		case '(':
        			pre_order_traverse_node(f);
        			cout << ")";
        			break;
        		}
			}

        	Token* process_primary_exp()
        	{
        		if(stringValue == "(" && process_token())
        		{
        			Token* tObjP = exp();
        			if(tObjP)
        			{
        				if(tokenObjectPs[tracker]->process_punctuator(")"))
        				{
    						add_childP(tObjP);
        					return this;
        				}
        			}

        		}
        		return nullptr;
        	}

        	Token* process_punctuator(const std::string &puncValue)
        	{
				if (puncValue == stringValue)
					switch (puncValue[0])
					{
					case '{':
					{
							process_token();
							std::vector<Token*> second_level = decl_list();

							std::vector<Token*> statements = stat_list();
							if(statements.empty())
								return nullptr; // it means there is an error in the block.

							second_level.insert(second_level.end(),statements.begin(),statements.end());

							if(tracker < tokenObjectPs.size() && tokenObjectPs[tracker]->get_string_value() == "}")
							{
								tracker++;
								//tObj->add_children(second_level);
								for (auto newChild : second_level) {
									add_childP(newChild);
								}
								return this;
							}
							break;
					}
					default:
						return process_token();
					}

				return nullptr;
        	}
        };

        class id:public StringBasedToken
        {
			// identifiers -- example:  sum
        public:
        	id(const std::string &idValue): StringBasedToken(idValue){  };
        	Token* process_id()
        	{
        		return process_token();
        	}
        	Token* process_primary_exp()
        	{
        		return process_id(); // return process_token();
        	}
        };

        class type_spec:public StringBasedToken
		{
			// 'char' | 'int' | 'float'
        public:
        	type_spec(const std::string &typeValue): StringBasedToken(typeValue){ };
		};

        class assignment_operator:public StringBasedToken
		{
			// '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
        public:
        	assignment_operator(const std::string &opValue): StringBasedToken(opValue){ };
        	Token* advance_past_assignment_operator() {
        		return process_token();
        	}
		};

		class conditional_operator:public StringBasedToken
		{
		public:
        	conditional_operator(const std::string &opValue): StringBasedToken(opValue){ };
			// ***** Complete this class if needed
		};

        class shift_operator:public StringBasedToken
		{
			// '<<' | '>>'
        public:
        	shift_operator(const std::string &opValue): StringBasedToken(opValue){ };
        	Token* advance_past_shift_operator() {
        		return process_token();
        	}
		};

        class additive_operator:public StringBasedToken
		{
			// '+' | '-'
        public:
        	additive_operator(const std::string &opValue): StringBasedToken(opValue){ };
			// ***** Complete this class if needed
		};

        class div_operator:public StringBasedToken
		{
			// '*' | '/' | '%'
        public:
        	div_operator(const std::string &opValue): StringBasedToken(opValue){ };
			// ***** Complete this class if needed
		};

        class unary_operator:public StringBasedToken
		{
			//  '~' | '!'
        public:
        	unary_operator(const std::string &opValue): StringBasedToken(opValue){ };

        	Token* process_unary()
        	{
        		Token* subTree;
				if(process_token() && (subTree = unary_exp()))
				{
					add_childP(subTree);
					return this;
				}
				return nullptr;
           	}
		};

		class postfix_operator:public StringBasedToken
		{
		public:
    		postfix_operator(const std::string &opValue): StringBasedToken(opValue){ };
    		void traverse_node(action_func f) const
			{
				Token::post_order_traverse_node(f);
			}
    		bool is_postfix_operator() { return true; }

		};

        class string:public StringBasedToken
        {
        	// e.g., "sample string"
        public:
        	string(const std::string &StringValue): StringBasedToken(StringValue){ };

        	Token* process_primary_exp()
        	{
        		return process_token();
        	}

        };



        class Const:public Token
		{
			// subclasses: int_const, char_const, float_const
        public:
        	Token* process_primary_exp()
        	{
        		return process_token();
        	}

		};

        class int_const:public Const
		{
		private:
			int intValue; // e.g., 1234
		public:
			int_const(const int& IntValue) { intValue=IntValue; }
			const int& getintValue() const { return intValue; }
			void print(std::ostream& str) const { Token::print(str); str << ": " << intValue; }
			void print_node_values() const { cout << intValue << " "; };
		};

        class char_const :public Const
		{
		private:
			char charValue; // e.g., 'a' or 127
		public:
			char_const(const char& CharValue) { charValue = CharValue; }
			const char& getcharValue() const { return charValue; }
			void print(std::ostream& str) const { Token::print(str); str << ": " << charValue; }
			void print_node_values() const { cout << charValue << " "; };
		};

        class float_const :public Const
		{
		private:
			float floatValue; // e.g., 12.3
		public:
			float_const(const float& FloatValue) { floatValue = FloatValue; }
			const float& getfloatValue() const { return floatValue; }
			void print(std::ostream& str) const {Token::print(str); str << ": " << floatValue; }
			void print_node_values() const { cout << floatValue << " "; };
        };



} // namespace ensc251

#endif /* PARSERCLASSES_H_ */
