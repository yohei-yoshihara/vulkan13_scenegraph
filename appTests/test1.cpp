#include <catch2/catch_test_macros.hpp>

int add(int a, int b) { return a + b; }

TEST_CASE("add test") { 
  // sample test case
  CHECK(add(1, 2) == 3);
}