/*
ARM GCC 15.2.0 with -O0 in compiler explorer:

struct MyClass
{
    int num;
};

int main ()
{
    MyClass obj{1};

    return obj.num;
}
 */
main:
        push    {r7}
        sub     sp, sp, #12
        add     r7, sp, #0
        movs    r3, #1
        str     r3, [r7, #4]
        ldr     r3, [r7, #4]
        mov     r0, r3
        adds    r7, r7, #12
        mov     sp, r7
        pop     {r7}
        bx      lr