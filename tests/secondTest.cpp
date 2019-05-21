#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

TEST_CASE( "SecondTestSample", "[second_test_sample]" ) {
    REQUIRE( 2 == 12 );
}