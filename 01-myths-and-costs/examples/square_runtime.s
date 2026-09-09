/*
ARM GCC 15.2.0 with -O0 in compiler explorer:

int square(int a)
{
    return a*a;
}

int main ()
{
    int ret = square(2);
    return ret;
}
*/
square(int):
        push    {r7}            @ set up stack frame
        sub     sp, sp, #12
        add     r7, sp, #0
        str     r0, [r7, #4]    @ 'a' arrives in r0
        ldr     r3, [r7, #4]
        mul     r3, r3, r3      @ this is 'a*a'
        mov     r0, r3          @ function return
        adds    r7, r7, #12     @ begin cleanup
        mov     sp, r7
        pop     {r7}
        bx      lr              @ go back to whoever called the function
main:
        push    {r7, lr}        @ setup
        sub     sp, sp, #8
        add     r7, sp, #0
        movs    r0, #2          @ pass 2 as the argument
        bl      square(int)     @ call the function
        str     r0, [r7, #4]    @ this is 'int ret = ...'
        ldr     r3, [r7, #4]
        mov     r0, r3          @ 'return ret'
        adds    r7, r7, #8      @ begin cleanup
        mov     sp, r7
        pop     {r7, pc}