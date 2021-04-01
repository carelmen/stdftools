//
// Created by carelmen on 25/06/20.
//

#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

//using namespace std;

using ::testing::Invoke;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::WithArg;


int main(int argc, char** argv) {
    // The following line must be executed to initialize Google Mock
    // (and Google Test) before running the tests.
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

