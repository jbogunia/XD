#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../class/Validation.cpp"

TEST_CASE( "ValidationTest", "[validation]" ) {
    SECTION("test title") {

        // too long title
        REQUIRE(!Validation::validateTitle("Too long title"));

        //good length
        REQUIRE(Validation::validateTitle("Title"));

        //more then 3 char, that are no letters (possibility of spam)
        REQUIRE(!Validation::validateTitle("SPAM!!!!!"));
    }
    SECTION("test body"){
        //no body test
        REQUIRE(!Validation::validateBody(""));

        //too long text in the body
        REQUIRE(!Validation::validateBody("This is really long text in the body, can be spam."));

        //good length
        REQUIRE(Validation::validateBody("This is fine."));

    }


    SECTION("test password"){
        //too short password
        REQUIRE(!Validation::validatePassword("sh1"));

        //too long password
        REQUIRE(!Validation::validatePassword("Toolongpassowrd1"));

        //no digit in password
        REQUIRE(!Validation::validatePassword("nodigit"));

        //good password
        REQUIRE(Validation::validatePassword("goodpass1"));

    }
}
