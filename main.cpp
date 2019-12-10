#include <QCoreApplication>
#include <iostream>

#include "mathexpression.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    double x = 0;
    double y = 2.71828;
    MathExpression test("((sin(x) ^ 2 + cos(x) ^ 2) * 2 - ln(y)) / 2");
    cout << test.calculate(x, y) << endl; // 0.5
    return a.exec();
}
