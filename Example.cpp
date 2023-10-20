#include <iostream>
#include "ReflectCPP.h"


struct MyClass
{
    RFL_CLASS(MyClass,
    {
        RFL_FIELD(MyClass, int, valueA);
        RFL_FIELD(MyClass, int, valueB);
        RFL_FIELD(MyClass, int, valueC);
    })


    MyClass(int a)
    {
    }


    int64_t somePadding = 1;
    int valueA = 10;
    int valueB = 10;
    int valueC = 10;
};

int main()
{

    auto fields = MyClass::StaticClass().GetClassReflector().GetFields();
    for (auto& It : fields)
    {
        std::cout << It.m_Name << " " << (uint32_t)It.m_Type << " " << It.m_Offset << std::endl;
    }

    return 0;
}
