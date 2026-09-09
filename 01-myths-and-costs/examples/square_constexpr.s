/*
ARM GCC 15.2.0 with -O0 in compiler explorer:

constexpr int square(int a)
{
    return a*a;
}

int main()
{
    constexpr int ret = square(2);
    return ret;
}
*/
main:
        push    {r7}            @ set up stack frame
        sub     sp, sp, #12
        add     r7, sp, #0
        movs    r3, #4          @ square(2) was already evaluated at compile time -> 4
        str     r3, [r7, #4]
        movs    r3, #4
        mov     r0, r3          @ function return
        adds    r7, r7, #12     @ begin cleanup
        mov     sp, r7
        pop     {r7}
        bx      lr