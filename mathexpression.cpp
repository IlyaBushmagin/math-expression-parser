#include "mathexpression.h"

MathExpression::MathExpression(const QString &mathExpression)
{
    setWeightTable();
    convertToRPN(mathExpression);
}

void MathExpression::setWeightTable(void)
{
    weightTable.insert("+", 1);
    weightTable.insert("-", 1);
    weightTable.insert("*", 2);
    weightTable.insert("/", 2);
    weightTable.insert("^", 3);
    weightTable.insert("sin", 4);
    weightTable.insert("cos", 4);
    weightTable.insert("ln", 4);
}

bool MathExpression::isBinaryOperation(const QString &operation)
{
    return operation == "+" || operation == "-" || operation == "*"
            || operation == "/" || operation == "^";
}

bool MathExpression::isUnaryOperation(const QString &operation)
{
    return operation == "sin" || operation == "cos" || operation == "ln";
}

void MathExpression::convertToRPN(const QString &mathExpression)
{
    bool ok;
    QString RPN_Element;
    for(auto it = mathExpression.begin(); it != mathExpression.end(); it++)
    {
        if((*it >= "0" && *it <= "9") || *it == ".")
        {
            RPN_Element.append(*it);
            RPN_Element.toDouble(&ok);
            if(ok && (it + 1) == mathExpression.end())
            {
                RPN.append(RPN_Element);
                RPN_Element.clear();
            }
        }
        else
        {
            RPN_Element.toDouble(&ok);
            if(ok)
            {
                RPN.append(RPN_Element);
                RPN_Element.clear();
            }
            else if(RPN_Element == "sin" || RPN_Element == "cos"
                    || RPN_Element == "ln")
            {
                addOperation(RPN_Element);
                RPN_Element.clear();
            }
            if(*it == "x" || *it == "y")
            {
                RPN.append(*it);
            }
            else if(*it == "+" || *it == "*" || *it == "/"
                    || *it == "^" || *it == "(" || *it == ")")
            {
                addOperation(*it);
            }
            else if(*it == "-")
            {
                if(it != mathExpression.begin() && *(it - 1) != "(")
                {
                    addOperation("+");
                    RPN.append("0");
                    addOperation("-");
                }
                else
                {
                    RPN.append("0");
                    addOperation("-");
                }
            }
            else if(*it != " ")
            {
                RPN_Element.append(*it);
            }
        }
    }
    while(!operationsStack.isEmpty())
    {
        RPN.append(operationsStack.pop());
        weightStack.pop_back();
    }
}

void MathExpression::addOperation(const QString &operation)
{
    if(operation == ")")
    {
        while(operationsStack.top() != "(")
        {
            RPN.append(operationsStack.pop());
            weightStack.pop_back();
        }
        operationsStack.pop_back();
        weightStack.pop_back();
        return;
    }
    if(operation == "(")
    {
        operationsStack.push(operation);
        weightStack.push(0);
        return;
    }
    int operationWeight = weightTable.value(operation);
    while(!operationsStack.isEmpty() && weightStack.top() >= operationWeight)
    {
        RPN.append(operationsStack.pop());
        weightStack.pop_back();
    }
    operationsStack.push(operation);
    weightStack.push(operationWeight);
}

double MathExpression::calculate(double x, double y)
{
    QStack<double> valuesStack;
    QVector<QString> RPN_copy = RPN;
    double operand_1, operand_2;
    while(!RPN_copy.isEmpty())
    {
        if(isBinaryOperation(RPN_copy.first()))
        {
            operand_2 = valuesStack.pop();
            operand_1 = valuesStack.pop();
            if(RPN_copy.first() == "+")
            {
                valuesStack.push(operand_1 + operand_2);
            }
            else if(RPN_copy.first() == "-")
            {
                valuesStack.push(operand_1 - operand_2);
            }
            else if(RPN_copy.first() == "*")
            {
                valuesStack.push(operand_1 * operand_2);
            }
            else if(RPN_copy.first() == "/")
            {
                valuesStack.push(operand_1 / operand_2);
            }
            else if(RPN_copy.first() == "^")
            {
                valuesStack.push(pow(operand_1, operand_2));
            }
        }
        else if(isUnaryOperation(RPN_copy.first()))
        {
            operand_1 = valuesStack.pop();
            if(RPN_copy.first() == "sin")
            {
                valuesStack.push(sin(operand_1));
            }
            else if(RPN_copy.first() == "cos")
            {
                valuesStack.push(cos(operand_1));
            }
            else if(RPN_copy.first() == "ln")
            {
                valuesStack.push(log(operand_1));
            }
        }
        else if(RPN_copy.first() == "x")
        {
            valuesStack.push(x);
        }
        else if(RPN_copy.first() == "y")
        {
            valuesStack.push(y);
        }
        else
        {
            valuesStack.push(RPN_copy.first().toDouble());
        }
        RPN_copy.removeFirst();
    }
    double result = valuesStack.pop();
    return result;
}

MathExpression::~MathExpression(){}
