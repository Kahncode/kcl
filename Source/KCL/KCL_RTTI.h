// MIT License
//
// Copyright(c) 2019 Samuel Kahn
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstdint>
#include <type_traits>

#include "KCL_Platform.h"
#include "KCL_Utils_Preprocessor.h"

// Minimalistic and efficient RTTI Implementation.
// Adds virtual methods to registered types, as polymorphism is necessary
// Dynamic casts cost in the worst case one virtual call and walking through a data buffer

// Note:
// * This is not safe to pass across boundaries.
// * This is not thread safe. The type info is created the first time it is accessed, race conditions may occur.

/*Usage :

//For all polymorphic types including base classes
class Type : public Base1, public Base2
{
public:
	KCL_RTTI_IMPL()
	//...
};

//For all types except types using inheritance
KCL_RTTI_REGISTER(Type)
//For types using inheritance
KCL_RTTI_REGISTER(Type, Base1, Base2 ...)

*/

namespace KCL
{
// Details, this is not meant to be used outside of this file
namespace RTTI_Private
{
// You can reduce the size at will here
typedef uint32_t typeId_t;

static typeId_t GenerateId()
{
	// magic number that increases every time it is called
	static typeId_t theTypeIdCounter = 0;
	return ++theTypeIdCounter;
}

template<typename Derived, typename Base>
static ptrdiff_t ComputePointerOffset()
{
	Derived* derivedPtr = (Derived*)1;
	Base* basePtr = static_cast<Base*>(derivedPtr);
	return (intptr_t)basePtr - (intptr_t)derivedPtr;
}

// Specialization will contain the magic data.
template<typename T>
struct TypeData
{
};

// Member ::ourTypeId will be TypeId instance
// Member ::ourInfo will be TypeInfo instance
template<typename T>
struct TypeInfoImpl
{
};

// Do NOT use! Use KCL::RTTI::GetTypeInfo instead
// Member ::Get() will return const TypeInfo*
template<typename T>
struct GetTypeInfo
{
};

#pragma pack(push, 1)

// Recursively populates the typeData
// Layout of typeData:
// [ typeId_t size, typeId_t firstTypeId ... typeId_t lastTypeId, ptrdiff_t offset/endMarker if = 0,
// typeId_t size, typeId_t firstTypeId ... typeId_t lastTypeId, ptrdiff_t offset/endMarker if = 0... ]
// Each block represents inherited types from a base, the first block doesn't need offset as it is implicitly 0
// Therefore we can use the offset as an end marker, all other bases will have a positive offset
template<typename... BaseTypes>
struct BaseTypeData
{
};

template<typename FirstBase, typename SecondBase, typename... Next>
struct BaseTypeData<FirstBase, SecondBase, Next...>
{
	template<typename Derived>
	void FillBaseTypeData(typeId_t& outSize, ptrdiff_t aOffset)
	{
		myFirst.FillBaseTypeData<Derived>(outSize, ComputePointerOffset<Derived, FirstBase>());

		myOffset = ComputePointerOffset<Derived, SecondBase>();
		myNext.FillBaseTypeData<Derived>(mySize, myOffset);
	}

	BaseTypeData<FirstBase> myFirst;
	ptrdiff_t myOffset;
	typeId_t mySize;
	BaseTypeData<SecondBase, Next...> myNext;
};

template<typename Base>
struct BaseTypeData<Base>
{
	template<typename Derived>
	void FillBaseTypeData(typeId_t& outSize, ptrdiff_t aOffset)
	{
		const TypeData<Base>* baseTypeId = (TypeData<Base>*)(GetTypeInfo<Base>::Get()->GetTypeData());

		// return size of head list
		outSize = baseTypeId->mySize;

		const char* data = baseTypeId->GetData();
		size_t byteSize = baseTypeId->mySize * sizeof(typeId_t);

		// copy type list
		memcpy(myData, data, byteSize);

		size_t byteIndex = byteSize;
		ptrdiff_t offset = *(ptrdiff_t*)(data + byteIndex);
		while (offset != 0)
		{
			// fill next offset
			*(ptrdiff_t*)(myData + byteIndex) = offset + aOffset;
			byteIndex += sizeof(ptrdiff_t);

			// fill next size
			const typeId_t size = *(typeId_t*)(data + byteIndex);
			*(typeId_t*)(myData + byteIndex) = size;
			byteSize = size * sizeof(typeId_t);
			byteIndex += sizeof(typeId_t);

			// copy types
			memcpy(myData + byteIndex, data + byteIndex, byteSize);
			byteIndex += byteSize;

			offset = *(ptrdiff_t*)(data + byteIndex);
		}
	}

	// Size minus end marker
	char myData[sizeof(TypeData<Base>) - sizeof(ptrdiff_t) - sizeof(typeId_t)];
};

// Actual implementation of the TypeId
template<typename Type, typename... BaseTypes>
struct TypeDataImpl
{
	TypeDataImpl()
	{
		myTypeId = GenerateId();
		myBaseTypes.FillBaseTypeData<Type>(mySize, 0);
		mySize++;
		myEndMarker = 0;
	}

	const char* GetData() const { return (char*)&myTypeId; }

	typeId_t mySize;
	typeId_t myTypeId;
	BaseTypeData<BaseTypes...> myBaseTypes;
	ptrdiff_t myEndMarker;
};

template<typename Type>
struct TypeDataImpl<Type>
{
	TypeDataImpl() : mySize(1), myTypeId(GenerateId()), myEndMarker(0) {}

	const char* GetData() const { return (char*)&myTypeId; }

	typeId_t mySize;
	typeId_t myTypeId;
	ptrdiff_t myEndMarker;
};

#pragma pack(pop)

} // namespace RTTI_Private

// Public RTTI API
namespace RTTI
{
typedef KCL::RTTI_Private::typeId_t typeId_t;

// Interface of TypeInfo
struct TypeInfo
{
	KCL_FORCEINLINE const char* GetName() const { return myName; }
	KCL_FORCEINLINE const char* GetTypeData() const { return (char*)(this + 1); }
	KCL_FORCEINLINE typeId_t GetTypeId() const { return *(typeId_t*)(GetTypeData() + sizeof(typeId_t)); }
	inline intptr_t CastTo(intptr_t aPtr, typeId_t aTypeId) const
	{
		const char* data = GetTypeData();
		size_t byteIndex = 0;
		ptrdiff_t offset = 0;

		while (true)
		{
			typeId_t size = *(typeId_t*)(data + byteIndex);
			byteIndex += sizeof(typeId_t);

			for (typeId_t i = 0; i < size; i++, byteIndex += sizeof(typeId_t))
			{
				if (*(typeId_t*)(data + byteIndex) == aTypeId)
					return aPtr + offset;
			}

			offset = *(ptrdiff_t*)(data + byteIndex);
			if (offset == 0)
				return 0;

			byteIndex += sizeof(ptrdiff_t);
		}
	}

	KCL_FORCEINLINE bool operator==(const TypeInfo& anOther) const { return GetTypeId() == anOther.GetTypeId(); }
	KCL_FORCEINLINE bool operator!=(const TypeInfo& anOther) const { return GetTypeId() != anOther.GetTypeId(); }

	const char* myName;
};

// Public interface to access type information
// Always access through this or risk wrong behavior
// General template is there to avoid duplicates when using combinations of the base type
template<typename T>
KCL_FORCEINLINE const TypeInfo* GetTypeInfo()
{
	typedef typename std::decay<std::remove_cv<T>::type>::type Type;
	return KCL::RTTI_Private::GetTypeInfo<Type>::Get();
}

template<typename T>
KCL_FORCEINLINE typeId_t GetTypeId()
{
	return GetTypeInfo<T>()->GetTypeId();
}

template<typename Derived, typename Base>
KCL_FORCEINLINE Derived DynamicCast(Base* aBasePtr)
{
	static_assert(std::is_pointer<Derived>::value, "Return type must be a pointer");
	typedef std::remove_pointer<Derived>::type DerivedObjectType;

	if constexpr (std::is_base_of<DerivedObjectType, Base>::value)
		return static_cast<Derived>(aBasePtr);
	else if (aBasePtr)
		return reinterpret_cast<Derived>(aBasePtr->KCL_RTTI_DynamicCast(GetTypeId<DerivedObjectType>()));
	else
		return nullptr;
}

} // namespace RTTI
} // namespace KCL

template<typename Derived, typename Base>
KCL_FORCEINLINE Derived kcl_dynamic_cast(Base* aBasePtr)
{
	return KCL::RTTI::DynamicCast<Derived, Base>(aBasePtr);
}

// Common declaration
// Note: Ideally we need to pack this structure as well, issues may appear
#define KCL_RTTI_TYPEINFO(TYPE)                                                                                                            \
	template<>                                                                                                                             \
	struct TypeInfoImpl<TYPE>                                                                                                              \
	{                                                                                                                                      \
		const KCL::RTTI::TypeInfo ourInfo;                                                                                                 \
		const TypeData<TYPE> ourTypeId;                                                                                                    \
	};                                                                                                                                     \
                                                                                                                                           \
	template<>                                                                                                                             \
	struct GetTypeInfo<TYPE>                                                                                                               \
	{                                                                                                                                      \
		static const KCL_FORCEINLINE KCL::RTTI::TypeInfo* Get()                                                                            \
		{                                                                                                                                  \
			static TypeInfoImpl<TYPE> ourInstance = {{#TYPE}, TypeData<TYPE>()};                                                           \
			return &ourInstance.ourInfo;                                                                                                   \
		}                                                                                                                                  \
	};

// Use for all types, must include all directly inherited types in the macro
// Note: Ideally we could remove the Register macro by making it possible to register it all from withing the class,
// but this approach allows registering non-class types as well.
#define KCL_RTTI_REGISTER(...)                                                                                                             \
	namespace KCL                                                                                                                          \
	{                                                                                                                                      \
	namespace RTTI_Private                                                                                                                 \
	{                                                                                                                                      \
	template<>                                                                                                                             \
	struct TypeData<KCL_FIRST_ARG(__VA_ARGS__)> : public TypeDataImpl<__VA_ARGS__>                                                         \
	{                                                                                                                                      \
	};                                                                                                                                     \
	KCL_RTTI_TYPEINFO(KCL_FIRST_ARG(__VA_ARGS__))                                                                                          \
	}                                                                                                                                      \
	}

// Use in the body of all polymorphic types
#define KCL_RTTI_IMPL()                                                                                                                    \
                                                                                                                                           \
	virtual intptr_t KCL_RTTI_DynamicCast(KCL::RTTI::typeId_t aOtherTypeId) const                                                          \
	{                                                                                                                                      \
		typedef std::remove_pointer<decltype(this)>::type ObjectType;                                                                      \
		return KCL::RTTI::GetTypeInfo<ObjectType>()->CastTo((intptr_t)this, aOtherTypeId);                                                 \
	}                                                                                                                                      \
	virtual const KCL::RTTI::TypeInfo* KCL_RTTI_GetTypeInfo() const                                                                        \
	{                                                                                                                                      \
		typedef std::remove_pointer<decltype(this)>::type ObjectType;                                                                      \
		return KCL::RTTI::GetTypeInfo<ObjectType>();                                                                                       \
	}                                                                                                                                      \
	virtual const char* KCL_RTTI_GetTypeName() const { return KCL_RTTI_GetTypeInfo()->GetName(); }                                         \
	virtual KCL::RTTI::typeId_t KCL_RTTI_GetTypeId() const                                                                                 \
	{                                                                                                                                      \
		typedef std::remove_pointer<decltype(this)>::type ObjectType;                                                                      \
		return KCL::RTTI::GetTypeId<ObjectType>();                                                                                         \
	}
