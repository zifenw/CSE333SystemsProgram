#include <iostream>
using namespace std;

class A {
public:
    virtual void foo() = 0;
    virtual void bar() { 
        baz();  // 调用 baz()
        cout << "A::bar" << endl; 
    }
    virtual void baz() { cout << "A::baz" << endl; }  // 非虚函数
};

class B : public A {
public:
    void foo() { 
        bar();  // 调用 bar()
        cout << "B::foo" << endl; 
    }
    void baz() { cout << "B::baz" << endl; }  // 这个不会被调用
};

int main() {
    B* b_ptr_b = new B();
    b_ptr_b->foo();  // 触发调用链
    delete b_ptr_b;
    return 0;
}
// #include <iostream>
// using namespace std;

// class Thing {
// public:
//     virtual void f() { cout << "Thing::f" << endl; }
//     void g() { cout << "Thing::g" << endl; }
//     void h() { g(); cout << "Thing::h" << endl; }
// };

// class Gadget : public Thing {
// public:
//     virtual void h() { cout << "Gadget::h" << endl; }
//     void j() { g(); cout << "Gadget::j" << endl; }
//     void f() { h(); cout << "Gadget::f" << endl; }
// };

// class Widget : public Gadget {
// public:
//     void g() { cout << "Widget::g" << endl; }
//     virtual void j() { f(); cout << "Widget::j" << endl; }
//     void h() { cout << "Widget::h" << endl; }
// };

// int main() {
//     Gadget *x = new Widget();
//     x->j(); // Calls Gadget::j()

//     // cout << "--- Additional Tests ---" << endl;
//     // Widget w;
//     // Gadget *g = &w;
//     // Thing *t = &w;
    
//     // g->f(); // Calls Widget::h() due to polymorphism, then prints "Gadget::f"
//     // t->f(); // Calls Thing::f because Thing::f is virtual, but not overridden in Widget
//     // g->h(); // Calls Widget::h() because h() is virtual
//     // t->h(); // Calls Thing::h(), which calls Thing::g() and prints "Thing::h"

//     delete x;
//     return 0;
// }
