#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED
#include <string>
#define MAXELEMENTS 50
#include "CustomStack.h"
using namespace std;

typedef CustomStack<char, MAXELEMENTS> CharStack;
typedef CustomStack<double, MAXELEMENTS> DoubleStack;

class Expression
{
    double result;
    Expression(const Expression & e);
	Expression& operator=(const Expression& e);
    void ProcessStack(CharStack& opStack, DoubleStack& argStack , int stop);
    int GetOperationPriority(char op);
    double PerformOperation(char op, double LeftArg, double RightArg);
public:
    Expression(const string&);
    string ResultAsString();

};



#endif // EXPRESSION_H_INCLUDED
