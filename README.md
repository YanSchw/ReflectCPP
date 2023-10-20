<p align="center">
  <a href="https://github.com/YanSchw/ReflectCPP">
    <img src="ReflectCPP.png" width="400" alt="ReflectCPP">
  </a>
</p>

A small and simple C++ Reflection Library

## Main Features
- Allocate any type of Class by its Id handle using std::new, if a default constructor is available
- Access primitive ClassMembers without knowing during the ClassType during compile-time
- Access RunTimeTypeInformation (**RTTI**) using little to none Boilerplate code  

## Examples
Allocate any type of Class by its Id handle, if a default constructor is available
```C++
#include "ReflectCPP.h"
using namespace rfl;

struct MyClass
{
    RFL_CLASS(MyClass);
    MyClass() = default;
    // ...
};

int main()
{
    Class cls = MyClass::StaticClass();

    // Create an instance anytime during runtime
    void* inst = cls.NewInstance();
}
```

Access primitive ClassMembers without knowing during the ClassType during compile-time
```C++
#include "ReflectCPP.h"
using namespace rfl;

struct MyClass
{
    RFL_CLASS(MyClass,
    {
        RFL_FIELD(int, myValue);
    });
    MyClass(int val)
        : myValue(val)
    {
    }
    
    int myValue;
};

int main()
{
    auto fields = MyClass::StaticClass().GetClassReflector().GetFields();
    for (auto& It : fields)
    {
        std::cout << It.m_Name << " " << (uint32_t)It.m_Type << " " << It.m_Offset << std::endl;
    }
}
```

## Requirements
- C++17 Compiler
- Include the ReflectCPP.h anywhere you want to use it
- Compile and link the ReflectCPP.cpp Translation Unit
- No external Libraries needed!
- Everything is pure C++
