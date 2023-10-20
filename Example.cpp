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
        valueA = a;
    }

    void PrintFields()
    {
        std::cout << "a: " << valueA << std::endl;
        std::cout << "b: " << valueB << std::endl;
        std::cout << "c: " << valueC << std::endl;
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

    MyClass inst = MyClass(13);

    std::cout << "Before: " << std::endl; inst.PrintFields();
    for (auto& It : fields)
    {
        int& ref = It.GetFieldRefFromObject<int>(&inst);
        ref++;
    }
    std::cout << "After: " << std::endl; inst.PrintFields();


    return 0;
}
