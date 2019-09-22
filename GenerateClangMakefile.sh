mkdir -p Build
cd Build
cmake -D CMAKE_C_COMPILER=clang-9 -D CMAKE_CXX_COMPILER=clang++-9 -D CMAKE_CXX_FLAGS="-O3" ..
