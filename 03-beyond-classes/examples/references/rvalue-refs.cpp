#include <cstdio>
#include <string>
#include <vector>

int main(void)
{
    std::string str = "Hello World, this is move semantics demo!!!";
    printf("str.data address is %p\r\n", (void*)str.data());

    std::vector<std::string> v;
    printf("str after copy is <%s>\r\n", str.data());

    v.push_back(std::move(str));
    // v.push_back(static_cast<std::string&&>(str));

    printf("str after move is <%s>\r\n", str.data());

    for(const auto &s : v)
    {
        printf("s is <%s>\r\n", s.data());
        printf("s.data address is %p\r\n", (void *)s.data());
    }

    return 0;
}