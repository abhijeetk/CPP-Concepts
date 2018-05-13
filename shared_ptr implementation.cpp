// https://codereview.stackexchange.com/questions/166395/custom-stdshared-ptrt-implementation
// https://codereview.stackexchange.com/questions/59004/simple-shared-pointer
// http://lokiastari.com/blog/2014/12/30/c-plus-plus-by-example-smart-pointer/
#include <iostream>
using namespace std;

template <class T>
class shared_ptr_custom {
private:
    T* ptr;
    int* count; // This should be pointer because similar to 'ptr' count value should
                // be same for all instance.

    void free() {
        if (ptr) {
            if (*this->count > 0) {
                (*this->count)--;
            }

            if (*count == 0) {
                cout << "destructing memory : " << ptr << endl;
                delete this->ptr;
                delete this->count;
            }

            this->count = nullptr;
            this->ptr = nullptr;
        }
    }
public:
    explicit shared_ptr_custom() : ptr(nullptr), count(nullptr) {
    
    }
    explicit shared_ptr_custom(T* ptr) : count(nullptr) {
        this->ptr = ptr;
        if (this->count == nullptr) {
            this->count = new int(1);
        }
    }

    shared_ptr_custom(const shared_ptr_custom<T>& rhs) {
        if (this->ptr != rhs.ptr) {
            this->ptr = rhs.ptr;
            this->count = rhs.count;    // Both current and rhs should have increamented value of
            (*this->count)++;           // count, but 'rhs' is const, we can not modify it.(rhs.count++) not allowed
        }                               // but this->count referes same memory.
    }

    const shared_ptr_custom<T>& operator = (const shared_ptr_custom<T>& rhs) {
        if (this->ptr != rhs.ptr) {
            this->free(); // I have given up ownership of previous pointer
            shared_ptr_custom<T> temp(rhs); // copy and swap idiom for assignment. It will copy construct object for me
            std::swap(this->count, temp.count);
            std::swap(this->ptr, temp.ptr);
        }
        return  *this;
    }

    shared_ptr_custom<T>(shared_ptr_custom<T>&& rhs) { // not passed as 'const', rhs is rvalue and temp
        std::swap(this->ptr, rhs.ptr);
        std::swap(this->count, rhs.count);
        rhs.ptr = nullptr;
        rhs.count = nullptr;
    }

    const shared_ptr_custom<T>& operator = (shared_ptr_custom<T>&& rhs) { // not passed as 'const'
        this->free(); // given up previous ownership and gaining new ownership
        std::swap(this->ptr, rhs.ptr);
        std::swap(this->count, rhs.count);
        rhs.ptr = nullptr;
        rhs.count = nullptr;
        return *this;
    }

    T* operator -> (void) {
        return ptr;
    }

    T operator * (void) {
        return *ptr;
    }

    ~shared_ptr_custom() {
        free();
    }

    unsigned int useCount() {
        return this && this->count != nullptr ? *this->count : 0;
    }
};

int main() {
    cout << "COPY" << endl;
    {
        // Only one memory allocation in this block
        // destructor should delete memory once;
        shared_ptr_custom<int> p(new int(10));  // copy constructor
        cout << p.useCount() << endl;
        shared_ptr_custom<int> a = p; // copy constructor
        cout << p.useCount() << endl;
        shared_ptr_custom<int> b; // copy assignment
        b = p;
        cout << p.useCount() << endl;
    }

    cout << "MOVE" << endl;
    {
        // Two memory allocation in this block
        // destructor should delete twice
        shared_ptr_custom<int> p1(std::move(new int(100)));
        cout << p1.useCount() << endl;
        shared_ptr_custom<int> a = std::move(p1);
        cout << a.useCount() << " : " <<p1.useCount() << endl;
        shared_ptr_custom<int> b(std::move(new int(1000)));
        shared_ptr_custom<int> b2 = b;
        cout << b.useCount() << endl;
        b = std::move(a);  // move assignment, given up previous ownership and gaining new ownership
        cout << b.useCount() << " : " << b2.useCount() << endl;
    }
    return 0;
}
