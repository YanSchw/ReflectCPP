#pragma once

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

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace rfl
{
    struct Class;
    struct ClassField;
    struct ClassReflector;
    using ClassID = std::uint64_t;

    enum class FieldType : uint32_t
    {
        None = 0,
        Int32,
        Int64
    };

    struct Class
    {
        static const Class None;
    private:
        ClassID m_ClassID = 0;
    public:
        explicit Class(ClassID id)
        {
            m_ClassID = id;
        }

        Class& operator=(ClassID id)
        {
            m_ClassID = id;
            return *this;
        }

        [[nodiscard]] ClassID GetClassID() const
        {
            return m_ClassID;
        }

        bool operator==(const Class& cls) const
        {
            return (*this == cls.m_ClassID);
        }
        bool operator==(ClassID id) const
        {
            return id == m_ClassID;
        }
        bool operator!=(const Class& cls) const
        {
            return !operator==(cls);
        }
        bool operator!=(ClassID id) const
        {
            return !operator==(id);
        }


        static std::vector<Class> GetAllClasses();

        std::size_t operator()(const Class& cls) const
        {
            return ((std::hash<ClassID>()(cls.m_ClassID)));
        }

        [[nodiscard]] std::string GetClassName() const;
        [[nodiscard]] const ClassReflector& GetClassReflector() const;

        static Class FromString(const std::string& str);

    };

    struct ClassField
    {
        ClassField(FieldType type, std::string name, std::size_t offset)
        : m_Type(type), m_Name(std::move(name)), m_Offset(offset) { }


        template<class T>
        T& GetFieldRefFromObject(void* obj)
        {
            assert(m_Type == GetFieldTypeFromTemplate<T>());
            return *(T*)(void*)(std::size_t(obj) + m_Offset);
        }

        template<class T>
        static FieldType GetFieldTypeFromTemplate();

        FieldType m_Type = FieldType::None;
        std::string m_Name;
        std::size_t m_Offset;
    };

    struct ClassReflector
    {
        static ClassReflector& Create(Class cls);

        void SetClassName(const std::string& name);
        [[nodiscard]] std::string GetClassName() const;

        void AddField(const ClassField& field);
        [[nodiscard]] std::vector<ClassField> GetFields() const;
    private:
        std::string m_ClassName;
        std::vector<ClassField> m_Fields;

        inline static std::unordered_map<ClassID, std::unique_ptr<ClassReflector>> s_AllClasses;
        friend struct Class;
    };

    struct ObjectFactory
    {
        template<class T>
        struct RegisterRflClass
        {
            RegisterRflClass()
            {
                s_ObjectAllocators[T::StaticClass().GetClassID()] = InternalAllocate<T>;
            }
        };

        static void* Allocate(ClassID id);

        ObjectFactory() = delete;
    private:

        inline static std::unordered_map<ClassID, void*(*)(void)> s_ObjectAllocators;

        template<class T>
        static void* InternalAllocate()
        {
            return new T();
        }

        friend struct Class;
    };

#define RFL_HASH(Value) ::std::hash<::std::string>()(Value)

#define RFL_FIELD(ClassName, FType, FName) desc.AddField(::rfl::ClassField(::rfl::ClassField::GetFieldTypeFromTemplate<FType>(), #FName, offsetof(ClassName, FName)))

#define RFL_CLASS_ID(ClassName, ClassId, ...)                           \
                                                                        \
class InternalAlloc;                                                    \
class InternalRTTI                                                      \
{                                                                       \
    friend class InternalAlloc;                                         \
    InternalRTTI()                                                      \
    {                                                                   \
        ::rfl::ClassReflector& desc = ::rfl::ClassReflector::Create(StaticClass());  \
        if constexpr (std::is_default_constructible<ClassName>::value)       \
        {                                                               \
            ::rfl::ObjectFactory::RegisterRflClass<ClassName> regiter = ::rfl::ObjectFactory::RegisterRflClass<ClassName>();\
        }                                                               \
        desc.SetClassName(#ClassName);                                   \
        __VA_ARGS__                                                     \
    }                                                                   \
                                                                        \
};                                                                      \
class InternalAlloc                                                     \
{                                                                       \
    inline static InternalRTTI Internal_Instance = InternalRTTI();      \
};                                                                      \
static ::rfl::Class StaticClass() { return ::rfl::Class(ClassId); }     \


#define RFL_CLASS(ClassName, ...) RFL_CLASS_ID(ClassName, RFL_HASH(#ClassName), __VA_ARGS__)

}

// For use in std::unordered_map
namespace std
{
    template <>
    struct hash<rfl::Class>
    {
        std::size_t operator()(const rfl::Class& cls) const
        {
            return ((std::hash<rfl::ClassID>()(cls.GetClassID())));
        }
    };
}