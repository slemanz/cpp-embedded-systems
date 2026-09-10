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
main:
        movs    r0, #1
        bx      lr