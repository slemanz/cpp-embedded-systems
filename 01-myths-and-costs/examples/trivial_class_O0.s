/*
ARM GCC 15.2.0 with -O0 in compiler explorer:

class MyClass
{
    private:
        int num;
    public:
        MyClass(int t_num):num(t_num){}
        ~MyClass(){}

        int getNum() const{
            return num;
        }
};

int main(void)
{
    MyClass obj(1);

    return obj.getNum();
}
 */
MyClass::MyClass(int) [base object constructor]:
        push    {r7}
        sub     sp, sp, #12
        add     r7, sp, #0
        str     r0, [r7, #4]
        str     r1, [r7]
        ldr     r3, [r7, #4]
        ldr     r2, [r7]
        str     r2, [r3]
        ldr     r3, [r7, #4]
        mov     r0, r3
        adds    r7, r7, #12
        mov     sp, r7
        pop     {r7}
        bx      lr
MyClass::~MyClass() [base object destructor]:
        push    {r7}
        sub     sp, sp, #12
        add     r7, sp, #0
        str     r0, [r7, #4]
        ldr     r3, [r7, #4]
        mov     r0, r3
        adds    r7, r7, #12
        mov     sp, r7
        pop     {r7}
        bx      lr
MyClass::getNum() const:
        push    {r7}
        sub     sp, sp, #12
        add     r7, sp, #0
        str     r0, [r7, #4]
        ldr     r3, [r7, #4]
        ldr     r3, [r3]
        mov     r0, r3
        adds    r7, r7, #12
        mov     sp, r7
        pop     {r7}
        bx      lr
main:
        push    {r4, r7, lr}
        sub     sp, sp, #12
        add     r7, sp, #0
        adds    r3, r7, #4
        movs    r1, #1
        mov     r0, r3
        bl      MyClass::MyClass(int) [complete object constructor]
        adds    r3, r7, #4
        mov     r0, r3
        bl      MyClass::getNum() const
        mov     r4, r0
        nop
        adds    r3, r7, #4
        mov     r0, r3
        bl      MyClass::~MyClass() [complete object destructor]
        mov     r3, r4
        mov     r0, r3
        adds    r7, r7, #12
        mov     sp, r7
        pop     {r4, r7, pc}