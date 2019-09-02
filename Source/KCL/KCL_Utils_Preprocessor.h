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

// clang-format off

// Preprocessor utilities
// Macros with _ prefix are considered implementation details and should not be used or relied upon

// Converts parameter to string, guarantees macro expansion 2 levels
// Ex: KCL_TOSTRING(arg) <=> "arg"
#define KCL_TOSTRING(arg)  _KCL_TOSTRING1(arg)
#define _KCL_TOSTRING1(arg) _KCL_TOSTRING2(arg)
#define _KCL_TOSTRING2(arg) #arg

// Concatenates macro parameters text to string, guarantees macro expansion 2 levels
#define KCL_CONCATENATE(arg1, arg2)   _KCL_CONCATENATE1(arg1, arg2)
#define _KCL_CONCATENATE1(arg1, arg2)  _KCL_CONCATENATE2(arg1, arg2)
#define _KCL_CONCATENATE2(arg1, arg2)  arg1 ## arg2

//Used most notably for MSVC's different behavior in handling __VA_ARGS__.
//In MSVC __VA_ARGS__ is expanded as a single token which means the next macro will only have one parameter
//To work around this remplace MACRO(__VA_ARGS__) by KCL_EXPAND(MACRO(__VA_ARGS)) which will perform correct substitution
//More background info in https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define KCL_EXPAND(va_args) va_args

// Returns the first argument of a variadic macro pack
#define _KCL_FIRST_ARG(FIRST, ...) FIRST
#define KCL_FIRST_ARG(...) KCL_EXPAND(_KCL_FIRST_ARG(__VA_ARGS__))

// Counts the number of variadic arguments
// Used with no arguments it will in fact return 1, this seems to be unavoidable as even an empty __VA_ARGS__ count as an argument to _KCL_VA_COUNT_IMPL
#define KCL_VA_COUNT(...) \
KCL_EXPAND(_KCL_VA_COUNT_EXPAND(__VA_ARGS__, _KCL_RSEQ_N()))

#define _KCL_VA_COUNT_EXPAND(...) \
KCL_EXPAND(_KCL_VA_COUNT_IMPL(__VA_ARGS__))

#define _KCL_VA_COUNT_IMPL( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,N,...) N

#define _KCL_RSEQ_N() \
63,62,61,60,                   \
59,58,57,56,55,54,53,52,51,50, \
49,48,47,46,45,44,43,42,41,40, \
39,38,37,36,35,34,33,32,31,30, \
29,28,27,26,25,24,23,22,21,20, \
19,18,17,16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0

// Foreach macro applies a macro to all of the following arguments
#define KCL_FOREACH(MACRO, ...) KCL_EXPAND(_KCL_FOREACH_IMPL(KCL_VA_COUNT(__VA_ARGS__), MACRO, __VA_ARGS__))

#define _KCL_FOREACH_1(MACRO, FIRST, ...) MACRO(FIRST)
#define _KCL_FOREACH_2(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_1(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_3(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_2(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_4(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_3(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_5(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_4(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_6(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_5(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_7(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_6(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_8(MACRO, FIRST, ...) MACRO(FIRST) KCL_EXPAND(_KCL_FOREACH_7(MACRO, __VA_ARGS__))

#define _KCL_FOREACH_IMPL(N, MACRO, ...) KCL_EXPAND(KCL_CONCATENATE(_KCL_FOREACH_, N)(MACRO, __VA_ARGS__))


// Foreach with a macro of two parameters
// Will not compile with odd number of parameters
#define KCL_FOREACH_2ARGS(MACRO, ...) KCL_EXPAND(_KCL_FOREACH_IMPL_2ARGS(KCL_VA_COUNT(__VA_ARGS__), MACRO, __VA_ARGS__))

#define _KCL_FOREACH_2ARGS_2(MACRO, FIRST, SECOND, ...)  MACRO(FIRST, SECOND)
#define _KCL_FOREACH_2ARGS_4(MACRO, FIRST, SECOND, ...)  MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_2 (MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_6(MACRO, FIRST, SECOND, ...)  MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_4 (MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_8(MACRO, FIRST, SECOND, ...)  MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_6 (MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_10(MACRO, FIRST, SECOND, ...) MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_8 (MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_12(MACRO, FIRST, SECOND, ...) MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_10(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_14(MACRO, FIRST, SECOND, ...) MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_12(MACRO, __VA_ARGS__))
#define _KCL_FOREACH_2ARGS_16(MACRO, FIRST, SECOND, ...) MACRO(FIRST, SECOND) KCL_EXPAND(_KCL_FOREACH_2ARGS_14(MACRO, __VA_ARGS__))

#define _KCL_FOREACH_IMPL_2ARGS(N, MACRO, ...) KCL_EXPAND(KCL_CONCATENATE(_KCL_FOREACH_2ARGS_, N)(MACRO, __VA_ARGS__))
