#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../class/ErrorResponse.cpp"
#include <typeinfo>

TEST_CASE( "ErrorResponseTest", "[errorResponse]" ) {
	
    SECTION("testing code 200") {
        ErrorResponse errorResponse(200);
        //REQUIRE(typeid(errorResponse.getCode()).name() ==  "i");
        REQUIRE(errorResponse.getCode() == 200);

        ErrorResponse errorResponse1(200, (char*)"OK");
        //REQUIRE(typeid(errorResponse1.getMessage()).name() == "char*");
        REQUIRE(errorResponse1.getMessage() == (char*)"OK");

    }

    SECTION("testing code 401") {
        ErrorResponse errorResponse(401);
        //REQUIRE(typeid(errorResponse.getCode()).name() ==  "i");
        REQUIRE(errorResponse.getCode() == 401);

        ErrorResponse errorResponse1 (401, (char*)"Unauthorized");
        //REQUIRE(typeid(errorResponse1.getMessage()).name() == "char*");
        REQUIRE(errorResponse1.getMessage() == (char*)"Unauthorized");

    }

    SECTION("testing code 500") {
        ErrorResponse errorResponse(500);
        //REQUIRE(typeid(errorResponse.getCode()).name() ==  "i");
        REQUIRE(errorResponse.getCode() == 500);

        ErrorResponse errorResponse1(500, (char*)"Internal Server Error");
        //REQUIRE(typeid(errorResponse1.getMessage()).name() =="char*");
        REQUIRE(errorResponse1.getMessage() == (char*)"Internal Server Error");

    }


}
