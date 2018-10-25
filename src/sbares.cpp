/*!
 * Converts an arithmetic expression in infix notation into postfix
 * representation. After the convertion is successfyly done, the
 * program evaluates the expression based on stacks and queues.
 *
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character** (all positives).
 * Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */

#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error
#include <vector>    // std::vector

//=== Aliases
using value_type = long int; //!< Type we operate on.
using symbol = char; //!< A symbol in this implementation is just a char.

// Simple helper functions that identify the incoming symbol.
bool is_operator( symbol s ){
	return std::string("x^*/%+-").find(s) != std::string::npos;
}

bool is_operand( symbol s ){
	return s >= '0' and s <= '9';
}

bool is_opening_scope( symbol s ){
	return s == '(';
}
bool is_closing_scope( symbol s ){
	return s == ')';
}

/// Check the operand's type of association.
bool is_right_association( symbol op ){
	return op == '^';
}

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

/// Converts a char (1-digit operand) into an integer.
value_type char2integer( char c ){
	return c - '0'; 
}



/// Change an infix expression into its corresponding postfix representation.
value_type evaluate_postfix( std::string );
/// Returns the precedence value (number) associated with an operator.
short get_precedence( symbol op )
{
	switch(op){
		case '^': return 3;
		case '*':
		case '/':
		case '%': return 2;
		case '+':
		case '-': return 1;
		case '(': return 0;
		default : assert (false); return -1;
	}
}

/// Determines whether the first operator is >= than the second operator.
bool has_higher_or_eq_precedence( symbol op1 , symbol op2 ){

	int p_op1 = get_precedence(op1);
	int p_op2 = get_precedence(op2);

	if(p_op1 > p_op2){
		return true;
	}

	else if(p_op1 < p_op2){
		return false;
	}

	else{
		if(is_right_association(op1) ){
			return false;
		} 
		return true;
	}
}

/// Execute the binary operator on two operands and return the result.
value_type execute_operator( value_type v1, value_type v2, symbol op ){
	switch(op)
	{
		case '^': return pow(v1,v2);
		case '*': return v1 * v2;
		case '/': if(v2 == 0) throw std::runtime_error("divisao por zero");
					return v1 / v2;
		case '%': if(v2 == 0) throw std::runtime_error("divisao por zero");
					return v1 % v2;
		case '+': return v1+v2;
		case '-': return v1-v2;

		default: throw std::runtime_error("undifined operation");
	}
}

int main( void )
{
    // A expression is a queue of sysmbols (chars).
    std::vector< std::string > exps {
        "4/(5^2)+(6^2^3)",
        "1+ 3 * ( 4 + 8 * 3 ^7)",
        "2*2*3",
        "2^2^3" } ;

    for ( const auto& e : exps )
    {
        auto postfix = infix_to_postfix( e );
        std::cout << ">>> Input (infix)    = " << e << "\n";
        std::cout << ">>> Output (postfix) = " << postfix << "\n";

        auto result = evaluate_postfix( postfix );
        std::cout << ">>> Result is: " << result << std::endl;
    }

    std::cout << "\n>>> Normal exiting...\n";
    return EXIT_SUCCESS;
}


std::string infix_to_postfix( std::string infix){

	std::string postfix(""); // Resultado da conversão
	std::stack< symbol > s; // Pilha de ajuda na conversão

	// Percorrer a entrada, para processar cada item

	for (auto c : infix)
	{
		if(is_operand( c)){
			postfix += c;
		}

		else if(is_opening_scope(c) ){
			s.push(c);
		}

		//desempilhando até achar o (
		else if(is_closing_scope(c)){
			while(not is_opening_scope( s.top() ) ){
				postfix += s.top();
				s.pop();
			}
			s.pop();
		}

		

		//Desempilhar as operações em espera que são iguais ou superiores

		else if(is_operator(c)){
			while(not s.empty() and has_higher_or_eq_precedence(s.top(), c) ){
				postfix += s.top();
				s.pop();
			}

			s.push( c );
		}

		//Ignorar entradas invalidas
		else{

		}
	}

	//Descarregar as operações pendentes
	while(not s.empty()){
		postfix += s.top();
		s.pop();
	}

	return postfix;
}


value_type evaluate_postfix( std::string postfix){

	std::stack<value_type> s;
	for (auto c : postfix){
		if(is_operand(c)){
			s.push( char2integer(c) );
		}
		else if(is_operator(c)){
			value_type op2 = s.top();
			s.pop();
			value_type op1 = s.top();
			s.pop();

			auto result = execute_operator(op1, op2, c);

			s.push( result );
		}
	}

	return s.top();
	return 0;
}