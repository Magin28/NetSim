#include <iostream>
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}