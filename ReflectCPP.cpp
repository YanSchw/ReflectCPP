#include "ReflectCPP.h"

/*
* MIT License
*
* Copyright(c) 2023 Yannick Schössow
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

namespace rfl
{

    const Class Class::None = Class((ClassID) 0);


    template<> FieldType ClassField::GetFieldTypeFromTemplate<int>() { return FieldType::Int32; }

    std::vector<Class> Class::GetAllClasses()
    {
        std::vector<Class> out;

        out.reserve(ClassReflector::s_AllClasses.size());
        for (auto& It : ClassReflector::s_AllClasses)
        {
            out.emplace_back(It.first);
        }

        return out;
    }

    const ClassReflector& Class::GetClassReflector() const
    {
        return *ClassReflector::s_AllClasses.at(GetClassID()).get();
    }

    std::string Class::GetClassName() const
    {
        return GetClassReflector().GetClassName();
    }

    Class Class::FromString(const std::string& str)
    {
        for (auto& It : ClassReflector::s_AllClasses)
        {
            if (It.second->GetClassName() == str)
            {
                return Class(It.first);
            }
        }

        return Class::None;
    }

    ClassReflector& ClassReflector::Create(Class cls)
    {
        ClassReflector::s_AllClasses[cls.GetClassID()] = std::make_unique<ClassReflector>();
        return *ClassReflector::s_AllClasses.at(cls.GetClassID()).get();
    }

    void* ObjectFactory::Allocate(ClassID id)
    {
        if (s_ObjectAllocators.find(id) == s_ObjectAllocators.end())
        {
            return nullptr;
        }
        return s_ObjectAllocators.at(id)();
    }

    void ClassReflector::SetClassName(const std::string &name)
    {
        m_ClassName = name;
    }
    std::string ClassReflector::GetClassName() const
    {
        return m_ClassName;
    }

    void ClassReflector::AddField(const ClassField &field)
    {
        m_Fields.push_back(field);
    }
    std::vector<ClassField> ClassReflector::GetFields() const
    {
        return m_Fields;
    }


}