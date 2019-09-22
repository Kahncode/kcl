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

#include "KCL_RTTI_Test.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

#include "KCL/KCL_RTTI.h"

//////////////////////////////////////////////////////////////////////////

#define BASE_CLASS(CLASS)                                                                                                                  \
	struct CLASS                                                                                                                           \
	{                                                                                                                                      \
		KCL_RTTI_IMPL() virtual ~CLASS() {}                                                                                                \
		int myInt##CLASS;                                                                                                                  \
	};                                                                                                                                     \
	KCL_RTTI_REGISTER(CLASS)

#define DERIVED_CLASS(CLASS, ...)                                                                                                          \
	struct CLASS : __VA_ARGS__                                                                                                             \
	{                                                                                                                                      \
		KCL_RTTI_IMPL() virtual ~CLASS() {}                                                                                                \
		int myInt##CLASS;                                                                                                                  \
	};                                                                                                                                     \
	KCL_EXPAND(KCL_RTTI_REGISTER(CLASS, __VA_ARGS__))

// Single inheritance hierarchies

BASE_CLASS(Base1)
DERIVED_CLASS(Derived1A, Base1)

DERIVED_CLASS(Derived2A, Derived1A)
DERIVED_CLASS(Derived3A, Derived2A)
DERIVED_CLASS(Derived4A, Derived3A)
DERIVED_CLASS(Derived5A, Derived4A)
DERIVED_CLASS(Derived6A, Derived5A)
DERIVED_CLASS(Derived7A, Derived6A)

DERIVED_CLASS(Derived1B, Base1)
DERIVED_CLASS(Derived2B, Derived1B)
DERIVED_CLASS(Derived3B, Derived2B)
DERIVED_CLASS(Derived4B, Derived3B)
DERIVED_CLASS(Derived5B, Derived4B)
DERIVED_CLASS(Derived6B, Derived5B)
DERIVED_CLASS(Derived7B, Derived6B)

DERIVED_CLASS(Derived1C, Base1)
DERIVED_CLASS(Derived2C, Derived1C)
DERIVED_CLASS(Derived3C, Derived2C)
DERIVED_CLASS(Derived4C, Derived3C)
DERIVED_CLASS(Derived5C, Derived4C)
DERIVED_CLASS(Derived6C, Derived5C)
DERIVED_CLASS(Derived7C, Derived6C)

BASE_CLASS(Base2)

DERIVED_CLASS(Derived1D, Base2)
DERIVED_CLASS(Derived2D, Derived1D)
DERIVED_CLASS(Derived3D, Derived2D)
DERIVED_CLASS(Derived4D, Derived3D)
DERIVED_CLASS(Derived5D, Derived4D)
DERIVED_CLASS(Derived6D, Derived5D)
DERIVED_CLASS(Derived7D, Derived6D)

DERIVED_CLASS(Derived1E, Base2)
DERIVED_CLASS(Derived2E, Derived1E)
DERIVED_CLASS(Derived3E, Derived2E)
DERIVED_CLASS(Derived4E, Derived3E)
DERIVED_CLASS(Derived5E, Derived4E)
DERIVED_CLASS(Derived6E, Derived5E)
DERIVED_CLASS(Derived7E, Derived6E)

DERIVED_CLASS(Derived1F, Base2)
DERIVED_CLASS(Derived2F, Derived1F)
DERIVED_CLASS(Derived3F, Derived2F)
DERIVED_CLASS(Derived4F, Derived3F)
DERIVED_CLASS(Derived5F, Derived4F)
DERIVED_CLASS(Derived6F, Derived5F)
DERIVED_CLASS(Derived7F, Derived6F)

// Multiple inheritance hierarchies

// Inherits from two long inheritance chains
DERIVED_CLASS(Multi1A, Base1, Base2)
DERIVED_CLASS(Multi2A, Derived1A, Derived1D)
DERIVED_CLASS(Multi3A, Derived2A, Derived2D)
DERIVED_CLASS(Multi4A, Derived3A, Derived3D)
DERIVED_CLASS(Multi5A, Derived4A, Derived4D)
DERIVED_CLASS(Multi6A, Derived5A, Derived5D)
DERIVED_CLASS(Multi7A, Derived6A, Derived6D)

// Inherits from all the previously defined hierarchies
// This diamond pattern is not adviced but KCL_RTTI should handle it as well as virtual inheritance
DERIVED_CLASS(Multi1B, Derived1A, Derived1B, Derived1C, Derived1D, Derived1E, Derived1F)
DERIVED_CLASS(Multi3B, Derived3A, Derived3B, Derived3C, Derived3D, Derived3E, Derived3F)
DERIVED_CLASS(Multi7B, Derived7A, Derived7B, Derived7C, Derived7D, Derived7E, Derived7F)

// Nested multi inheritance scenario

BASE_CLASS(Base3)
BASE_CLASS(Base4)
BASE_CLASS(Base5)
BASE_CLASS(Base6)

DERIVED_CLASS(Multi1C, Base1, Base2)
DERIVED_CLASS(Multi2C, Base3, Base4)
DERIVED_CLASS(Multi3C, Base5, Base6)
DERIVED_CLASS(Multi4C, Multi1C, Multi2C, Multi3C)
DERIVED_CLASS(Multi5C, Multi2C, Multi3C, Multi1C)
DERIVED_CLASS(Multi6C, Multi3C, Multi1C, Multi2C)

//////////////////////////////////////////////////////////////////////////

namespace KCL_Test
{
class Forward;
}

// Ensuring we can use RTTI with a type that is not yet declared
KCL_RTTI_REGISTER(KCL_Test::Forward)

namespace KCL_Test
{
class Forward
{
	KCL_RTTI_IMPL();
	int i;
};

void RTTI_Test()
{
	using namespace KCL::RTTI;

	assert(GetTypeId<Derived1A>() != GetTypeId<Base1>());
	assert(GetTypeInfo<Derived1A>() != GetTypeInfo<Base1>());

	// Testing type info validity with modifiers
	assert(GetTypeInfo<Forward>() == GetTypeInfo<const Forward>());
	assert(GetTypeInfo<Forward>() == GetTypeInfo<volatile Forward>());
	assert(GetTypeInfo<Forward>() == GetTypeInfo<const Forward&>());

	// Testing basic dynamic cast
	Base1 b;
	Derived1A d;
	Base1* basePtrToDerived = &d;
	Derived1A* dPtr = kcl_dynamic_cast<Derived1A*>(basePtrToDerived);
	assert(dPtr);

	dPtr = kcl_dynamic_cast<Derived1A*>(&b);
	assert(!dPtr);

	//standard dynamic_cast also support references
	Base1& bRef = b;
	Derived1A& dRef = kcl_dynamic_cast<Derived1A&>(bRef);
	assert((intptr_t)&dRef);

	Base1& bRef2 = *basePtrToDerived;
	Derived1A& dRef2 = kcl_dynamic_cast<Derived1A&>(bRef2);
	assert((intptr_t)&dRef2 != 0);

	{
		// confirming that deep inheritance chains work and the logic of recursive typeId building is sound
		Derived7A der7;
		Base1* base = kcl_dynamic_cast<Base1*>(&der7);
		Derived1A* der = kcl_dynamic_cast<Derived1A*>(&der7);
		Derived2A* der2 = kcl_dynamic_cast<Derived2A*>(&der7);
		Derived6A* der6 = kcl_dynamic_cast<Derived6A*>(&der7);
		assert(&der7 == base && base == der && base == der2 && base == der6);
	}

	{
		// confirm dynamic cast is working in multiple inheritance scenario
		Multi5A m;

		Base1* base1dyn = kcl_dynamic_cast<Base1*>(&m);
		Base2* base2dyn = kcl_dynamic_cast<Base2*>(&m);
		assert(base1dyn && base2dyn && (intptr_t)base1dyn != (intptr_t)base2dyn);

		Derived4A* der4A = kcl_dynamic_cast<Derived4A*>(base1dyn);
		Derived2D* der2D = kcl_dynamic_cast<Derived2D*>(base2dyn);
		Derived7A* der7A = kcl_dynamic_cast<Derived7A*>(base1dyn);
		assert(der4A && der2D && !der7A);
	}

	{
		// multiple inheritance, verify we are correctly using static_cast to cast to base

		Multi1A m;
		Base1* base1dyn = kcl_dynamic_cast<Base1*>(&m);
		Base1* base1static = static_cast<Base1*>(&m);
		Base2* base2dyn = kcl_dynamic_cast<Base2*>(&m);
		Base2* base2static = static_cast<Base2*>(&m);

		assert(base1dyn && base1static && base1dyn == base1static && (intptr_t)base1dyn == (intptr_t)&m);
		assert(base2dyn && base2static && base2dyn == base2static && (intptr_t)base2dyn != (intptr_t)&m);
	}

	{
		Multi4C m;

		// Test casting to all inherited bases from m ptr and Base1 ptr

		Base1* base1dyn = kcl_dynamic_cast<Base1*>(&m);
		Base1* base1static = static_cast<Base1*>(&m);
		assert(base1dyn == base1static && base1dyn == &m);

		Base2* base2dyn = kcl_dynamic_cast<Base2*>(base1dyn);
		Base2* base2dyn2 = kcl_dynamic_cast<Base2*>(&m);
		Base2* base2static = static_cast<Base2*>(&m);

		assert(base2dyn == base2static && base2dyn == base2static && base2dyn != nullptr);

		Base3* base3dyn = kcl_dynamic_cast<Base3*>(base1dyn);
		Base3* base3dyn3 = kcl_dynamic_cast<Base3*>(&m);
		Base3* base3static = static_cast<Base3*>(&m);

		assert(base3dyn == base3static && base3dyn == base3static && base3dyn != nullptr);

		Base4* base4dyn = kcl_dynamic_cast<Base4*>(base1dyn);
		Base4* base4dyn4 = kcl_dynamic_cast<Base4*>(&m);
		Base4* base4static = static_cast<Base4*>(&m);

		assert(base4dyn == base4static && base4dyn == base4static && base4dyn != nullptr);

		Base5* base5dyn = kcl_dynamic_cast<Base5*>(base1dyn);
		Base5* base5dyn5 = kcl_dynamic_cast<Base5*>(&m);
		Base5* base5static = static_cast<Base5*>(&m);

		assert(base5dyn == base5static && base5dyn == base5static && base5dyn != nullptr);

		Base6* base6dyn = kcl_dynamic_cast<Base6*>(base1dyn);
		Base6* base6dyn6 = kcl_dynamic_cast<Base6*>(&m);
		Base6* base6static = static_cast<Base6*>(&m);

		assert(base6dyn == base6static && base6dyn == base6static && base6dyn != nullptr);

		// Test casting to base1 ptr from all the other pointers should work so the back offset calculation is good

		Base1* base1dynfrom2 = kcl_dynamic_cast<Base1*>(base2dyn);
		assert(base1dyn == base1dynfrom2);
		Base1* base1dynfrom3 = kcl_dynamic_cast<Base1*>(base3dyn);
		assert(base1dyn == base1dynfrom3);
		Base1* base1dynfrom4 = kcl_dynamic_cast<Base1*>(base4dyn);
		assert(base1dyn == base1dynfrom4);
		Base1* base1dynfrom5 = kcl_dynamic_cast<Base1*>(base5dyn);
		assert(base1dyn == base1dynfrom5);
		Base1* base1dynfrom6 = kcl_dynamic_cast<Base1*>(base6dyn);
		assert(base1dyn == base1dynfrom6);

		// Same test with Base3

		Base3* base3dynfrom1 = kcl_dynamic_cast<Base3*>(base1dyn);
		assert(base3dyn == base3dynfrom1);
		Base3* base3dynfrom2 = kcl_dynamic_cast<Base3*>(base2dyn);
		assert(base3dyn == base3dynfrom2);
		Base3* base3dynfrom4 = kcl_dynamic_cast<Base3*>(base4dyn);
		assert(base3dyn == base3dynfrom4);
		Base3* base3dynfrom5 = kcl_dynamic_cast<Base3*>(base5dyn);
		assert(base3dyn == base3dynfrom5);
		Base3* base3dynfrom6 = kcl_dynamic_cast<Base3*>(base6dyn);
		assert(base3dyn == base3dynfrom6);

		// Same test with Base4

		Base4* base4dynfrom1 = kcl_dynamic_cast<Base4*>(base1dyn);
		assert(base4dyn == base4dynfrom1);
		Base4* base4dynfrom2 = kcl_dynamic_cast<Base4*>(base2dyn);
		assert(base4dyn == base4dynfrom2);
		Base4* base4dynfrom3 = kcl_dynamic_cast<Base4*>(base3dyn);
		assert(base4dyn == base4dynfrom3);
		Base4* base4dynfrom5 = kcl_dynamic_cast<Base4*>(base5dyn);
		assert(base4dyn == base4dynfrom5);
		Base4* base4dynfrom6 = kcl_dynamic_cast<Base4*>(base6dyn);
		assert(base4dyn == base4dynfrom6);

		// Same test with Base6

		Base6* base6dynfrom1 = kcl_dynamic_cast<Base6*>(base1dyn);
		assert(base6dyn == base6dynfrom1);
		Base6* base6dynfrom2 = kcl_dynamic_cast<Base6*>(base2dyn);
		assert(base6dyn == base6dynfrom2);
		Base6* base6dynfrom3 = kcl_dynamic_cast<Base6*>(base3dyn);
		assert(base6dyn == base6dynfrom3);
		Base6* base6dynfrom6 = kcl_dynamic_cast<Base6*>(base6dyn);
		assert(base6dyn == base6dynfrom6);
		Base6* base6dynfrom5 = kcl_dynamic_cast<Base6*>(base5dyn);
		assert(base6dyn == base6dynfrom5);

		// Check we allow unrelated types to be attempted (and possibly failed)
		Forward* forwardDyn = kcl_dynamic_cast<Forward*>(base1dyn);
		assert(!forwardDyn);
	}

	// Note: this will result in ambiguous conversion which is expected
	// Multi7B m;
	// Base1* base1dyn = kcl_dynamic_cast<Base1*>(&m);
}

static int validCastCounter = 0;

template<typename Derived, typename T>
KCL_NOINLINE void RunDynamicCastTest(const std::vector<std::shared_ptr<T>>& testVector, int loopCount)
{
	for (int i = 0; i < loopCount; i++)
	{
		for (const auto& it : testVector)
		{
			Derived* result = dynamic_cast<Derived*>(it.get());
			if (result)
				validCastCounter++;
		}
	}
}

template<typename Derived, typename T>
KCL_NOINLINE void RunKCLCastTest(const std::vector<std::shared_ptr<T>>& testVector, int loopCount)
{
	for (int i = 0; i < loopCount; i++)
	{
		for (const auto& it : testVector)
		{
			Derived* result = kcl_dynamic_cast<Derived*>(it.get());
			if (result)
				validCastCounter++;
		}
	}
}

void RTTI_Benchmark()
{
	using namespace std;
	using namespace chrono;

	static const int iterations = 1000000;
	static const int loopCount = 10;

	validCastCounter = 0;

	// One level deep single inheritance hierarchies
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived1A>());
			testObjects.emplace_back(make_shared<Derived1B>());
			testObjects.emplace_back(make_shared<Derived1C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 1 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 1 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 1 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 1 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Three level deep single inheritance hierarchies
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived3A>());
			testObjects.emplace_back(make_shared<Derived3B>());
			testObjects.emplace_back(make_shared<Derived3C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 3 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 3 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived3A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 3 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived3A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 3 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Seven level deep single inheritance hierarchies
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived7A>());
			testObjects.emplace_back(make_shared<Derived7B>());
			testObjects.emplace_back(make_shared<Derived7C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 7 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 7 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 7 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Single inheritance, 7 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Wrong cast, one level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived1A>());
			testObjects.emplace_back(make_shared<Derived1B>());
			testObjects.emplace_back(make_shared<Derived1C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 1 level deep. STD i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 1 level deep. KCL i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}
	}

	// Wrong cast, 3 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived3A>());
			testObjects.emplace_back(make_shared<Derived3B>());
			testObjects.emplace_back(make_shared<Derived3C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 3 level deep. STD i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 3 level deep. KCL i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}
	}

	// Wrong cast, 7 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Derived7A>());
			testObjects.emplace_back(make_shared<Derived7B>());
			testObjects.emplace_back(make_shared<Derived7C>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 7 level deep. STD i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Wrong cast, 7 level deep. KCL i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}
	}

	// nullptr cast
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(nullptr);
			testObjects.emplace_back(nullptr);
			testObjects.emplace_back(nullptr);
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Nullptr cast STD i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Multi1A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Nullptr cast KCL i: %zu, time (ms): %f\n", testObjects.size(), deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base1, 1 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi1A>());
			testObjects.emplace_back(make_shared<Multi1A>());
			testObjects.emplace_back(make_shared<Multi1A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 1 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 1 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 1 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 1 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base2, 1 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base2>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi1A>());
			testObjects.emplace_back(make_shared<Multi1A>());
			testObjects.emplace_back(make_shared<Multi1A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 1 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 1 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived1D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 1 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived1D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 1 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base1, 3 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi3A>());
			testObjects.emplace_back(make_shared<Multi3A>());
			testObjects.emplace_back(make_shared<Multi3A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 3 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 3 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived3A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 3 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived3A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 3 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base2, 3 level deep
	{
		// Prepare test vector
		vector<shared_ptr<Base2>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi3A>());
			testObjects.emplace_back(make_shared<Multi3A>());
			testObjects.emplace_back(make_shared<Multi3A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 3 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 3 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived3D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 3 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived3D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 3 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base1, 7 levels deep for greater effect
	{
		// Prepare test vector
		vector<shared_ptr<Base1>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi7A>());
			testObjects.emplace_back(make_shared<Multi7A>());
			testObjects.emplace_back(make_shared<Multi7A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 7 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base1>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 7 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 7 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived7A>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base1, 7 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}
	}

	// Multiple inheritance, base2, 7 levels deep for greater effect
	{
		// Prepare test vector
		vector<shared_ptr<Base2>> testObjects;
		testObjects.reserve(iterations * 3);

		for (int i = 0; i < iterations; i++)
		{
			testObjects.emplace_back(make_shared<Multi7A>());
			testObjects.emplace_back(make_shared<Multi7A>());
			testObjects.emplace_back(make_shared<Multi7A>());
		}

		// Upcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 7 level deep. STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Upcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Base2>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 7 level deep. KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast std
		{
			auto before = steady_clock::now();

			RunDynamicCastTest<Derived7D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 7 level deep. STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Downcast kcl
		{
			auto before = steady_clock::now();

			RunKCLCastTest<Derived7D>(testObjects, loopCount);

			auto after = steady_clock::now();
			duration<double, std::milli> deltaTime = after - before;

			printf("Multiple inheritance, base2, 7 level deep. KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
				deltaTime.count() / (float)loopCount);
		}

		// Multiple inheritance 3*2
		{
			// Prepare test vector
			vector<shared_ptr<Base1>> testObjects;
			testObjects.reserve(iterations * 3);

			for (int i = 0; i < iterations; i++)
			{
				testObjects.emplace_back(make_shared<Multi4C>());
				testObjects.emplace_back(make_shared<Multi5C>());
				testObjects.emplace_back(make_shared<Multi6C>());
			}

			// Upcast std
			{
				auto before = steady_clock::now();

				RunDynamicCastTest<Base4>(testObjects, loopCount);

				auto after = steady_clock::now();
				duration<double, std::milli> deltaTime = after - before;

				printf("Nested Multiple Inheritance 3*2 STD Upcast i: %zu, time (ms): %f\n", testObjects.size(),
					deltaTime.count() / (float)loopCount);
			}

			// Upcast kcl
			{
				auto before = steady_clock::now();

				RunKCLCastTest<Base4>(testObjects, loopCount);

				auto after = steady_clock::now();
				duration<double, std::milli> deltaTime = after - before;

				printf("Nested Multiple Inheritance 3*2 KCL Upcast i: %zu, time (ms): %f\n", testObjects.size(),
					deltaTime.count() / (float)loopCount);
			}

			// Downcast std
			{
				auto before = steady_clock::now();

				RunDynamicCastTest<Multi4C>(testObjects, loopCount);

				auto after = steady_clock::now();
				duration<double, std::milli> deltaTime = after - before;

				printf("Nested Multiple Inheritance 3*2 STD Downcast i: %zu, time (ms): %f\n", testObjects.size(),
					deltaTime.count() / (float)loopCount);
			}

			// Downcast kcl
			{
				auto before = steady_clock::now();

				RunKCLCastTest<Multi4C>(testObjects, loopCount);

				auto after = steady_clock::now();
				duration<double, std::milli> deltaTime = after - before;

				printf("Nested Multiple Inheritance 3*2 KCL Downcast i: %zu, time (ms): %f\n", testObjects.size(),
					deltaTime.count() / (float)loopCount);
			}
		}
	}

	printf("Valid cast counter: %d", validCastCounter);
}
} // namespace KCL_Test
