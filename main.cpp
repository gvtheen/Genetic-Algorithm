#include <iostream>
#include "CGaparameter.h"
#include "CGAEngine.h"
using namespace std;
using namespace GAZJUT;
double test_Function(const std::vector<double>& _data)
{
     double x1=_data[0];
     double x2=_data[1];
     return 0.5 + (sin(x1*x1-x2*x2)*sin(x1*x1-x2*x2) - 0.5)/((1+0.001*(x1*x1+x2*x2))*(1+0.001*(x1*x1+x2*x2)));
}
int main()
{
    CGaparameter* _ga_para = new CGaparameter();
     CGAEngine* _ga_Engine = new CGAEngine(_ga_para,&test_Function);

    _ga_Engine->init();
    _ga_Engine->evolve();
    delete _ga_Engine;
    delete _ga_para;
    return 0;
}
