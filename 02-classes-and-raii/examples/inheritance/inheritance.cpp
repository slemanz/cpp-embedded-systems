#include <cstdint>
#include <cstdio>

class A {
    public:
        void method_1()
        {
            printf("Class A, method1\r\n");
        }

        void method_2()
        {
            printf("Class A, method2\r\n");
        }

    protected:
        void method_protected()
        {
            printf("Class A, method_protected\r\n");
        }
};

class B : public A {
    public:
        void method_1()
        {
            printf("Class B, method1\r\n");
        }
       
        void method_3()
        {
            printf("Class B, method3\r\n");
            A::method_2();
            A::method_protected();
        }
};

int main(void)
{
    B b;

    b.method_1();
    b.method_2();
    b.method_3();

    printf("-----------------\r\n");

    A &a = b;

    a.method_1();
    a.method_2();

    return 0;
}