#include "Expression.h"
#include <iostream>
#include <cmath>
#include "functions.cpp"
using namespace std;

/** \brief Конструктур на класа, грижи се за изчисляването.
 *
 * Конструктура на класа използва два стека, попълва ги с
 * аргументите и операциите от подадения израз и го смята.
 * Записва резултата в атрибута result.
 *
 * \param const string& exp - израз за изчисляване
 */
Expression::Expression(const string& exp)
{
    CharStack opStack;
    DoubleStack argStack;
    size_t nextLeftBracket = 1;
    size_t nextRightBracket = exp.find('}');
    size_t exp_size = exp.size();
    nextRightBracket = (nextRightBracket && nextRightBracket < exp_size) ? nextRightBracket : exp_size;
    size_t bracketRange = nextRightBracket - nextLeftBracket -1;
    argStack.Push(StringToNumeric<double>(exp.substr(nextLeftBracket+1,bracketRange)));
    size_t i = nextRightBracket+1;

	while( exp[i] != '\0')
	{
	    nextLeftBracket = exp.find('{',i);
	    nextRightBracket = exp.find('}',i);
	     nextRightBracket = (nextRightBracket && nextRightBracket < exp_size ) ? nextRightBracket : exp_size;
	    bracketRange = nextRightBracket - nextLeftBracket -1;
		char newOp = exp[i];
		double newArg = StringToNumeric<double>(exp.substr(nextLeftBracket+1, bracketRange));
        ProcessStack(opStack, argStack, GetOperationPriority(newOp));

		opStack.Push(newOp);
		argStack.Push(newArg);
		i = nextRightBracket+1;
		if(i > exp_size)
            break;
	}

	ProcessStack(opStack, argStack, 0);
	this->result =  argStack.Peek();
}

/** \brief Връща приоритет на операцията
 *
 * Връща като целочислено число приоритета на операцията.
 *
 * \param char op - операция.
 * \return int - приоритет.
 */
int Expression::GetOperationPriority(char op)
{
    switch(op)
	{
		case '+':
		case '-':
			return 1;
        break;
		case '*':
		case '/':
			return 2;
        break;
		case '^':
			return 3;
        break;
	}
	return 0;
}

/** \brief Извършване на операция.
 *
 * Извършва операцията и я връща като резултат.
 *
 * \param char op - операция.
 * \param double LeftArg - ляв аргумент.
 * \param double RightArg - десен аргумент.
 * \return double - резултат от пресмятането.
 */
double Expression::PerformOperation(char op, double LeftArg, double RightArg)
{
	switch(op)
	{
		case '+':
			return LeftArg + RightArg;

		case '-':
			return LeftArg - RightArg;

		case '*':
			return LeftArg * RightArg;

		case '/':
			return LeftArg / RightArg;

		case '^':
			return pow(LeftArg, RightArg);
	}
	return 0;
}

/** \brief Чете от стековете и изпълнява операциите върху аргументите.
 *
 * Чете от стековите CharStack ( операции ) и ги изпълнява върху аргументите
 * от стека DoubleStack докато не стигне до операция с по-нисък приоритет от stop.
 *
 * \param CharStack& opStack - стек с операции.
 * \param DoubleStack& argStack - стек с аргументи.
 * \param int stop - приоритет за спиране.
 * \return void
 */
void Expression::ProcessStack(CharStack& opStack, DoubleStack& argStack, int stop)
{
	char op;
	double leftArg;
	double rightArg;

	while( ! opStack.IsEmpty() && GetOperationPriority(opStack.Peek()) > stop)
	{
		opStack.Pop(op);
		argStack.Pop(rightArg);
		argStack.Pop(leftArg);
		double value = PerformOperation(op, leftArg, rightArg);
		argStack.Push(value);
	}
}

/** \brief Връща резултата като стринг.
 *
 * След изчисляването връща резултата от атрибута result
 * като стринг.
 *
 *\return string - резултата.
 */
string Expression::ResultAsString()
{
    return DoubleToString(this->result);
}
