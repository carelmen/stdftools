# stdftools
Some tools for STDF files

## Conan
1. Create conan package for libstdf
cd conan/freestdf
conan create .


2. Change to your build directory
cd ../build

conan install ..
cmake ..
make -j

If conan install .. fails because of missing gtest, try using 
conan install .. --build=gtest



