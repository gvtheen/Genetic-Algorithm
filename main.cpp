#include <iostream>
#include "CGaparameter.h"
#include "CGAEngine.h"
using namespace std;
using namespace GAZJUT;
int main()
{
    CGaparameter* _ga_para = new CGaparameter();
    //cout << "Hello world-1!" << endl;
    CGAEngine* _ga_Engine= new CGAEngine(_ga_para);

    //cout << "Hello world-2!" << endl;
    _ga_Engine->init();
    _ga_Engine->evolve();
    delete _ga_Engine;
    delete _ga_para;
    cout << "Hello world!" << endl;
    return 0;
}
