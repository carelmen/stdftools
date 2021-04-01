cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# set(CONAN_DISABLE_CHECK_COMPILER True)

conan_basic_setup()

add_executable( runtests runtests.cpp)
target_link_libraries(runtests  gtest gmock pthread )

#add_executable( stdf_example example.cpp)
#target_link_libraries( stdf_example stdf)

add_executable( dump_records_to_ascii dump_records_to_ascii.c)
target_link_libraries( dump_records_to_ascii stdf)


# target_include_directories(example PUBLIC /usr/include)
# target_link_libraries(example -static stdf z bz2)

message("Include paths: ${CMAKE_INCLUDE_PATH}")

