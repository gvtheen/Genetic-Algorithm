#include <iostream>
#include "CGaparameter.h"
#include "CGAEngine.h"
using namespace std;
using namespace GAZJUT;
int main()
{
    CGaparameter* _ga_para = new CGaparameter();
       CGAEngine* _ga_Engine= new CGAEngine(_ga_para);

    _ga_Engine->init();
    _ga_Engine->evolve();
    delete _ga_Engine;
    delete _ga_para;
    return 0;
}
