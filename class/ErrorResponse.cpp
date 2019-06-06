class ErrorResponse {
private:
    int code;
    String message;
public:
    ErrorResponse(int code) {
        this->code = code;
    }

    ErrorResponse(int code, String message) {
        this->code = code;
        this->message = message;
    }

    int getCode(){
        return code;
    }

    const String getMessage() {
        return message;
    }

    const String getJsonMessage(){
      return "{\"message\" : \"" + this->getMessage()+ "\"}";
    }
};
