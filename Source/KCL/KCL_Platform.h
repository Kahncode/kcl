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

//////////////////////////////////////////////////////////////////////////
// Compiler and language detection
//////////////////////////////////////////////////////////////////////////

// KCL Compiler defines
// Useful link: http://sourceforge.net/p/predef/wiki/Compilers
#if defined(_MSC_VER)
#	define KCL_COMPILER_MSVC
#	define KCL_COMPILER_MSVC_2013 (_MSC_VER >= 1800)
#	define KCL_COMPILER_MSVC_2015 (_MSC_VER >= 1900)
#	define KCL_COMPILER_MSVC_2017 (_MSC_VER >= 1910)
#elif defined(__clang__)
#	define KCL_COMPILER_CLANG
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__) // clang also defines these
#	define KCL_COMPILER_GCC
#else
#	error Compiler not recognized
//#elif defined(__ICC) || defined(__INTEL_COMPILER)
/* Intel ICC/ICPC. ------------------------------------------ */
//#elif defined(__HP_cc) || defined(__HP_aCC)
/* Hewlett-Packard C/aC++. ---------------------------------- */
//#elif defined(__IBMC__) || defined(__IBMCPP__)
/* IBM XL C/C++. -------------------------------------------- */
//#elif defined(__PGI)
/* Portland Group PGCC/PGCPP. ------------------------------- */
//#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
/* Oracle Solaris Studio. ----------------------------------- */
#endif

// C++ language support
#if !defined(__cplusplus)
#	error This library must be compiled by a C++ compiler
#endif

#if (__cplusplus >= 201703L)
#	define KCL_CPPLANG 17
#elif (__cplusplus >= 201402L)
#	define KCL_CPPLANG 14
#elif (__cplusplus >= 201103L)
#	define KCL_CPPLANG 11
#else
#	define KCL_CPPLANG 03
#endif

// MSVC still defines __cplusplus to be 199711L (valid for MSVC 2017), better use _MSVC_LANG
#if defined(KCL_COMPILER_MSVC)
#	undef KCL_CPPLANG

#	if (_MSVC_LANG >= 201703L)
#		define KCL_CPPLANG 17
#	elif (_MSVC_LANG >= 201402L)
#		define KCL_CPPLANG 14
#	else
#		define KCL_CPPLANG 11 // _MSVC_LANG did not even exist prior to MSVC 2015 which is fully C++11
#	endif

#endif

// Check minimum required language version
#if !defined(KCL_CPPLANG)
#	error No C++ language support detected
#elif (KCL_CPPLANG < 14)
#	error C++14 compatible compiler required
#endif

//////////////////////////////////////////////////////////////////////////
// Platform specific configuration and utils
//////////////////////////////////////////////////////////////////////////

// Compiler behavior
#if defined(KCL_COMPILER_MSVC)
// Improve inlining behaviour of compiler
#	pragma inline_depth(255)
#	pragma inline_recursion(on)
#	pragma auto_inline(on)
#elif defined(KCL_COMPILER_CLANG)
// TODO : inlining behavior
#	pragma clang diagnostic ignored "-Wcomment" // disable nested comments warning
#else
#	error Not implemented for this compiler
#endif

// UE : check corresponding Platform.h for implementation details
#if defined(KCL_COMPILER_MSVC)
#	define KCL_FORCEINLINE __forceinline
#	define KCL_NOINLINE __declspec(noinline)
#	define KCL_ALIGN(X) MS_ALIGN(X)
#	define KCL_API_IMPORT_IMPL __declspec(dllimport)
#	define KCL_API_EXPORT_IMPL __declspec(dllexport)
#elif defined(KCL_COMPILER_CLANG)
#	define KCL_FORCEINLINE inline __attribute__((always_inline))
#	define KCL_NOINLINE __attribute__((noinline))
#	define KCL_ALIGN(X) GCC_ALIGN(X)
#elif defined(KCL_COMPILER_CGG)
#	define KCL_FORCEINLINE inline __attribute__((always_inline))
#	define KCL_NOINLINE __attribute__((noinline))
#	define KCL_ALIGN(X) GCC_ALIGN(X)
#else
#	error Not implemented for this compiler
#endif

//////////////////////////////////////////////////////////////////////////
// C++ Language Support
//////////////////////////////////////////////////////////////////////////

// Polyfill for upcoming language features
#if (KCL_CPPLANG >= 17)
#	define KCL_NODISCARD [[nodiscard]]
#	define KCL_FALLTHROUGH [[fallthrough]]
#	define KCL_UNUSED [[maybe_unused]]
#else
#	if defined(KCL_COMPILER_MSVC)
#		define KCL_NODISCARD
#		define KCL_FALLTHROUGH
#		define KCL_UNUSED
#	elif defined(KCL_COMPILER_CLANG)
#		define KCL_NODISCARD __attribute__((warn_unused_result))
#		define KCL_UNUSED __attribute__((unused))
#		define KCL_FALLTHROUGH
#	endif
#endif
