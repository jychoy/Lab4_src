//============================================================================
// Name        : parser.cpp
// Author      : Shams Nasinh
// Description : Recursive Descent Parser Starter Code with AST
//============================================================================
#include <string>
#include <vector>
#include <algorithm>

#include <iostream>

#define USE_OSS

#ifndef USE_OSS
#define OUT std::cout
#else
#include <sstream>
std::ostringstream oss;
#define OUT oss
#endif


#include "parserClasses.h"
using namespace std;

bool is_float(string &tokenValue)
{
		std::istringstream iss(tokenValue);
	    float f;
	    iss >> noskipws >> f;

	    return iss.eof() && !iss.fail();
}


bool is_int(string &s)
{
	   if(s.empty()) return false ;

	   char * p ;
	   errno = 0;
	   strtol(s.c_str(), &p, 10) ;
	   /*
		You should not check for errors by examining the return value of strtol,
		because the string might be a valid representation of
		 0l, LONG_MAX, or LONG_MIN. Instead, check whether tailptr points
		 to what you expect after the number (e.g. '\0' if the string
		 should end after the number). You also need to clear errno before
		 the call and check it afterward, in case there was overflow.
	    */

	   return (*p == 0) && !errno;
}

// Construct the token objects and categorize them into appropriate types
// Input: a string vector of tokens
// Output: a vector including the list of Token objects properly categorized
std::vector<ensc251::Token*> tokenClassifier(std::vector<std::string> tokenList)
{
        std::vector<ensc251::Token*> tokenObjectPs;

        vector<string> assign_op = { "=","*=","/=","%=","+=","-=","<<=",">>=","^=","|="};
        string unary_ops = "&~!"; // ! was not requested and is optional.
        string punctuators = ";{}():*[],"; // fine if '[' and ']' are missing because they were not in the comments

        for (std::string tokenValue : tokenList) {

                if(tokenValue == "char" || tokenValue == "int" || tokenValue == "float")
                {
                	auto tokenP = new ensc251::type_spec(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                } else if(tokenValue == "<<" || tokenValue == ">>")
                {
                	auto tokenP = new ensc251::shift_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                } else if(tokenValue[0] == '?')
				{
                	auto tokenP = new ensc251::conditional_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(tokenValue == "<<" || tokenValue == ">>")
                {
                	auto tokenP = new ensc251::shift_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                } else if(tokenValue == "+" || tokenValue == "-")
                {
                	auto tokenP = new ensc251::additive_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(tokenValue == "/" || tokenValue == "%")
                {
                	auto tokenP = new ensc251::div_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(tokenValue == "++" || tokenValue == "--")
                {
                	auto tokenP = new ensc251::postfix_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(unary_ops.find(tokenValue)!=string::npos)
                {
                	auto tokenP = new ensc251::unary_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(std::find(assign_op.begin(), assign_op.end(), tokenValue) != assign_op.end())
                {
                	auto tokenP = new ensc251::assignment_operator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                } else if(tokenValue[0] == '"')
				{
                	auto tokenP = new ensc251::string(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if(punctuators.find(tokenValue)!=string::npos)
				{
                	auto tokenP = new ensc251::punctuator(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if (is_int(tokenValue))
                {
                	auto tokenP = new ensc251::int_const(std::atoi(tokenValue.c_str()));
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if (is_float(tokenValue))
                {
                	auto tokenP = new ensc251::float_const(std::atof(tokenValue.c_str()));
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }else if (tokenValue[0] == '\'')
                {
                	auto tokenP = new ensc251::char_const(tokenValue[1]);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);

                }else
				{
                	// perhaps do some sanity test here
                	auto tokenP = new ensc251::id(tokenValue);
                	/*tokenP->print(std::cout);
                	cout << endl;*/
                	tokenObjectPs.push_back(tokenP);
                }

        }
        return tokenObjectPs;
}

#ifndef DONT_RUN_MAIN
int main() {

	//std::vector<std::string> tokenList= {"{","int","n",";","char","*","b",";","n","++","++",";","n","=","3","/","(","2","+","~","8",")",";","b","=","\"String\"", ",", "\"String2\"", ";","n",">>","5",";","n","=","n","?","n","+","5",":","1",";","}"};

	//std::vector<std::string> tokenList= {"{","errno","=","5",";","}"};
	std::vector<std::string> tokenList= {"{","int","=","5",";","}"}; // input used to create tree

	ensc251::tokenObjectPs = tokenClassifier(tokenList); // classify the tokens
	ensc251::Token* ast_root = ensc251::recursive_parser(); // this creates the nodes for the tree
	if(ast_root)
	{
		ensc251::action_func f = static_cast<ensc251::action_func> (ensc251::Token::print_node_values); // assign f to point to print_node function
		ast_root->traverse_node(f); // travel through the tree and print it
	}
	else
	{
		cout << "Syntax error in input. Please try again..." << endl;
		return 1;
	}

	return 0;
}
#endif
