#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H

#include <QString>
#include <QStack>
#include <QHash>
#include <cmath>

class MathExpression
{
public:
    MathExpression(const QString &mathExpression);

    double calculate(double x, double y);

    ~MathExpression();

private:
    void convertToRPN(const QString &mathExpression);

    void addOperation(const QString &operation);

    void setWeightTable(void);

    bool isUnaryOperation(const QString &operation);

    bool isBinaryOperation(const QString &operation);

    QHash<QString, int> weightTable;

    QStack<QString> operationsStack;

    QStack<int> weightStack;

    QVector<QString> RPN;
};

#endif // MATHEXPRESSION_H
