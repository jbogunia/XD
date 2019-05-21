#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../class/SampleClass.cpp"

TEST_CASE( "TestSampleClass", "[stest_sample_class]" ) {
    SampleClass obj;
    obj.set_values(3,4);
    REQUIRE( obj.area() == 12 );
}