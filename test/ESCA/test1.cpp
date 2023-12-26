// утечки паямти 10 ошибок
// ESCA - 8 нашел 1 ложная
// cppcheck - 3 нашел 0 ложных
// pvs - 6 нашел и 1 ложное
// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s
#include <cassert>

// нашли все | ESCA + | pvs +
void test_foo1() {
    // CHECK-DAG: resource leak. Variable name: x, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *x = new int[3];
    x[ 2 ] = 2;
}

// cppcheck - | ESCA + | pvs -
void test_foo2( int y ) {
    assert(y > 0);
    // CHECK-DAG: resource leak. Variable name: a, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *a = new int[y];
    a[ 0 ] = y;
    int *b1 = a;
    a = new int[y];
    a[ 0 ] = b1[ 0 ];
    delete[] a;
}

void getPath( char **p ) {
    *p = (char *) "filename";
}

// PVS и Cppcheck - | ESCA +
void getFile() {
    // CHECK-DAG: resource leak. Variable name: path, location: {{.*}}test1.cpp:[[# @LINE + 1]]:18
    char *path = new char[256];
    getPath(&path);
}

int *create1() {
    int *p = new int[42];
    return p;
}

// cppcheck + | ESCA +
int test_foo3_1() {
    // CHECK-DAG: resource leak. Variable name: p, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *p = create1();
    return p[ 2 ];
}

// cppcheck - | ESCA + | pvs +
int test_foo3_2( int x ) {
    // CHECK-DAG: resource leak. Variable name: a, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *a = create1();
    if( a[ x ] > 10 ) {
        // CHECK-DAG: resource leak. Variable name: a, location: {{.*}}test1.cpp:[[# @LINE + 1]]:13
        a = new int[2];
    }
    return *a;
}

// cppcheck + | ESCA + | pvs +
void test_foo4_1( int a ) {
    // CHECK-DAG: resource leak. Variable name: b, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *b = new int[a];
    if( a > 5 ) {
        b = new int[5];
    }
    b[ 2 ] = 5;
    delete[] b;
    // -leak b
}

// cppcheck + | ESCA + | pvs +
// note: ESCA does not find a leak here?
void test_foo4_2( bool a ) {
    int *b = new int[123];
    b[ 10 ] = 2;
    if( a ) {
        return;
    }
    delete[] b;
}

// cppcheck - | ESCA - | pvs +
void test_foo5( int a ) {
    int *b = nullptr;
    if( a == 1 ) {
        b = new int[123];
    }

    if( a - 1 ) {
        delete[] b;
    }
    // -leak b
}

// никто не нашел
void test_foo6( unsigned int sz ) {
    int *b[sz];
    for( int i = 0; i < sz; ++i ) {
        b[ i ] = new int[i];
    }
    for( int i = 0; i < sz - 1; ++i ) {
        delete b[ i ];
    }
}

void clear( int *a ) {
    delete[] a;
}

void test_foo7() {
    // ложное срабатывание ESCA + и pvs +
    // CHECK-DAG: resource leak. Variable name: a, location: {{.*}}test1.cpp:[[# @LINE + 1]]:14
    int *a = new int[10];
    clear(a);
}

// CHECK-DAG: Found 8 problems

int main() {
    test_foo1();
    test_foo2(10);
    test_foo3_1();
    test_foo3_2(2);
    test_foo4_1(2);
    test_foo4_2(true);
    test_foo5(2);
    test_foo6(2);
    test_foo7();
    getFile();
    return 0;
}
