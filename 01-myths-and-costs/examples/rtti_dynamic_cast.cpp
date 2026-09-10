#include <cstdio>

struct Base {
    virtual void print()
    {
        printf("Base\r\n");
    }
};

struct Derived : public Base {
    void print() override{
        printf("Derived\n\r");
    }
};

void printer(Base &base)
{
    base.print();
}

int main(void)
{
    Base base;
    Derived derived;

    printer(base);
    printer(derived);

    return 0;
}