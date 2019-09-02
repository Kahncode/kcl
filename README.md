# KCL - Kahncode Core Library

A suite of C++ core constructs and idioms that may or may not be useful to you.

Every part of KCL will be released following an article on http://kahncode.com/

KCL is open-source and released under the license found in the LICENSE file at the root of the repository. 
Contributions and comments are welcome. Please use GitHub (https://github.com/Kahncode/kcl) or contact me directly (samuel@kahncode.com).

KCL is concieved from the point of view of a game developer, and therefore the solutions offered may be more opinionated than the standard.
KCL focuses on speed, performance, modernity and expressivity of the code.
Best practices of game development apply. In order to allow a consistent behavior across all platforms and compilers, it avoids usage of the standard library as much as possible. Some features of C++ are intentionally not supported, such as RTTI or exceptions, as they are generally considered bad practice in game development.
KCL stays within what the c++ language can do without external tooling. Third-Party dependencies, code generation or external processing are therefore excluded.
KCL strives to support the lowest common denominator for game development. Currently the target is set to Visual Studio 2017 (MSVC Toolset v141) as all other compilers for relevant game development platforms are more standard compliant.

KCL is not intended to be production code, or to be included as-is into a project. This is merely a showcase. In order to use it properly, you should integrate, customize and extend it to fit your specific needs.

I hope that it is helpful to the game development and the c++ community at large.

## How to use

- Install CMake: https://cmake.org/
	- Make sure to add CMake to the PATH during installation
- (Windows) Run GenerateVisualStudioProject.bat
- (Bash) Run GenerateMakeFile.sh
- (Other) Use CMake and your preffered build system. Please contribute your changes to the CMake files are I am certain it can be improved.
