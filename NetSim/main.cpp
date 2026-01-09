#include <iostream>
#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}