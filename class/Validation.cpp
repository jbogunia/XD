#include <stdio.h>
#include <string.h>

class Validation {
public:
    static bool validateTitle(const char* title){
        //Possible length of title = 10
        if(strlen(title) > 10){
            return false;
        }
        //Checking if title consist of no letters
        int noLetterCounter = 0;
        for(int i = 0; title[i] != '\0'; i++){
            if((title[i] < 'a' || title[i] > 'z' )  &&  (title[i] < 'A' || title[i] > 'Z'))
                noLetterCounter++;
        }
        if(noLetterCounter > 3){
            return false;
        }
        return true;
    }
    static bool validateBody(const char* body){
        // body length 0-50
        if(strlen(body) > 20 || strlen(body) == 0){
            return false;
        }
        return true;
    }

    //Possible length of password from 8 to 20
    static bool validatePassword(const char* password){
        if(strlen(password) < 4 || strlen(password) > 10){
            return false;
        }
        //count digits in password
        int digitCounter = 0;
        for (int i = 0; password[i] != '\0'; i++) {
            if (password[i] >= '0' && password[i] <= '9')
                digitCounter++;
        }
        //no digit included in password
        if(digitCounter == 0){
            return false;
        }
        return true;

    }

};
