/*
    In embedded devices where stack size is less, object construction on stack me cause stack
    overflow. Defining class with private destructor force object of class to be created on heap
*/

class OpenFile {
public:
    OpenFile() {
    }

    void destoryMe() {
        delete this;
    }
private:
    ~OpenFile() {
    }
};

int main() {
    OpenFile* f = new OpenFile();
    f->destoryMe();
}