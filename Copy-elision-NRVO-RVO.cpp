// http://en.cppreference.com/w/cpp/language/copy_elision
// https://www.youtube.com/watch?v=Sv0h9W1OJjI
#include <iostream>
using namespace std;

class A {
public :
    A() { cout << "ctor : " << this << endl; }
    A(const A& ref) { cout << "copy ctor form" << &ref << "to " << this << endl; }
    A& operator = (const A& ref) { 
        cout << "copy assignment operator from " << &ref << "to " << this << endl;
        return *this;
    }
    //A(const A&& ref) {
    //    cout << "move ctor from " << &ref << " to " << this << endl;
    //}
    //A& operator = (const A&& ref) {
    //    cout << "move assignment operator from " << &ref << " to " << this << endl;
    //    return *this;
    //}
};

A f() {
    A a;      // Named
    return a;
}

A g() {
    return A();     // No Name
}

int main() {
    cout << "Compiler is not able to do return value optimization" << endl;
    A a1;
    a1 = f();

    cout << endl << "[NRVO] Named Return Value Optimization" << endl;
    A a2 = f();

    cout << endl << "Compiler is not able to do return value optimization" << endl;
    A a3;
    a3 = g();

    cout << endl << "[RVO] Return Value Optimization" << endl;
    A a4 = g();
    /*
    Compiler is not able to do return value optimization
    ctor : 0018FD0F
    ctor : 0018FBEF
    copy ctor form0018FBEFto 0018FC1F
    copy assignment operator from 0018FC1Fto 0018FD0F

    [NRVO] Named Return Value Optimization
    ctor : 0018FBEF
    copy ctor form0018FBEFto 0018FD03

    Compiler is not able to do return value optimization
    ctor : 0018FCF7
    ctor : 0018FC13
    copy assignment operator from 0018FC13to 0018FCF7

    [RVO] Return Value Optimization
    ctor : 0018FCEB

    */
    return 0;
}