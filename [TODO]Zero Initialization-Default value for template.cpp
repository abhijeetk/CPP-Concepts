#include <iostream>
using namespace std;

class A {

};

template <typename T>
class Foo
{
    T value {}; // How to initilalize value in generic way
public:
    Foo(T v) : value(v) {
    
    }
    T GetFoo() {
        return value;
    }
};

int main() {

    Foo<int> foo1(1);
    cout << foo1.GetFoo() << endl;

    A a;
    Foo<A> foo2(a);
    foo2.GetFoo();
    return 0;
}