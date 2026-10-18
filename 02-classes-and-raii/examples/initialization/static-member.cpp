#include <cstdio>
#include <cstdint>

struct object_counter
{
    static int cnt;

    object_counter()
    {
        cnt++;
    }

    ~object_counter()
    {
        cnt--;
    }
};

int object_counter::cnt = 0;

int main(void)
{
    {
        object_counter obj1;
        object_counter obj2;
        object_counter obj3;

        printf("Number of existing objects in this scope is: %d\r\n", object_counter::cnt);
    }

    printf("Number of existing objects in this scope is: %d\r\n", object_counter::cnt);
    
    return 0;
}