# Conan Package FreeSTDF
[Conan](https://conan.io/) package for the excellent [FreeSTDF](https://freestdf.sourceforge.io/).
Hopefully this is useful for somebody :) 

## Conan
1. Create conan package for freestdf
```
cd conan/freestdf
conan create . 
```

2. Change to your build directory
```
cd ../build 
conan install ..
cmake ..
make -j
```
If conan install .. fails because of missing gtest, try using 
```
conan install .. --build=gtest
```

You don't need gtest, just added it to test out conan.
There are probably a few things to clean up - compile works Debian (and Raspberry Pi) but failed on FreeBSD 
due to the bzip/gzip library dependencies. To be fixed.


