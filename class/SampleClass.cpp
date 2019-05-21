class SampleClass {
    int width, height;
public:
    void set_values (int,int);
    int area() {return width*height;}
};

void SampleClass::set_values (int x, int y) {
    width = x;
    height = y;
};