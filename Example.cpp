#include <iostream>
#include "ReflectCPP.h"


struct MyClass
{
    RFL_CLASS(MyClass)

    MyClass(int a)
    {

    }


    int value = 10;
};

int main()
{

    return 0;
}
