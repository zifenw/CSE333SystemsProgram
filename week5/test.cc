#include <iostream>
#include <cstdlib>
using namespace std;
class Dbl {
public:
// constructors and destructors
Dbl() {cval_=0.0; cout<<"default("<<cval_<<")"<<endl;}
Dbl(double n) {cval_=n; cout<<"ctor("<<cval_<<")"<<endl;}
Dbl(const Dbl& n) {
cval_ = n.cval_;
cout << "copyctor(" << cval_ << ")" << endl;
}
~Dbl() { cout << "dtor(" << cval_ << ")" << endl; }
// assignment operators
Dbl& operator=(const Dbl & other) {
if (this != &other) {
cval_ = other.cval_;
}
cout << "assign(" << other.cval_ << ")" << endl;
return *this;
}
Dbl& operator+=(const Dbl& other) {
cval_ += other.cval_;
cout << "op+=(" << cval_ << ")" << endl;
return *this;
}
private:
double cval_;
};
// additional overloaded operator
Dbl operator+(const Dbl &x, const Dbl &y) {
cout << "op+" << endl;
Dbl result = x;
result += y;
return result;
}

int main(int argc, char* argv[]) {
Dbl a = 1.0;
Dbl b(1.5);
Dbl c = b;
cout << "---" << endl;
a = 1 + c;
cout << "---" << endl;
a = b = c += 2;
return EXIT_SUCCESS;
}