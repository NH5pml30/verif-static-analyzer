// тестирование исключений в конструкторе и деструкторе
// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

#include <stdexcept>

class Database {
    int *x = new int[1];
public:
    void clear() {
        delete[] x;
    };
};

// исключение в деструкторе
// ESCA нашел PVS и cppcheck нет
class AA {
    Database *db;
    int *res = nullptr;
public:
    AA() : db(new Database()) {
        // CHECK-DAG: resource leak. Variable name: res, location: {{.*}}test4.cpp:[[# @LINE + 1]]:15
        res = new int[10];
    }

    void doSome() {
        // doSome
        delete db;
        db = nullptr;
    }

    ~AA() {
        clearDB();
        delete db;
        delete[]res;
    }

private:
    void clearDB() {
        if( !db ) {
            throw std::runtime_error("no databse");
        }
        db->clear();
    }
};

// pvs и cppcheck ESCA нашли
class SomeBadClass {
    int *x;
    bool thr;
public:
    SomeBadClass() {
        // CHECK-DAG: resource leak. Variable name: x, location: {{.*}}test4.cpp:[[# @LINE + 1]]:13
        x = new int[10];
    }

    // throwing destructor
    // CHECK-DAG: {{.*}}test4.cpp:[[# @LINE + 1]]:5: warning: throw in destructor 'SomeBadClass::~SomeBadClass'
    ~SomeBadClass() {
        if( thr ) {
            throw std::runtime_error("sdsf");
        }
        delete[] x;
    }
};

// CHECK-DAG: Found 3 problems

int main() {
    AA *a = new AA();
    a->doSome();
    delete a;
    SomeBadClass s;
}
