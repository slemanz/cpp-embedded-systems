#include <cstdint>
#include <cstdio>

struct uart
{
    uart(std::uint32_t baud = 9600): baudrate_(baud){}
    std::uint32_t baudrate_;
};

void uart_consumer(uart u)
{
    printf("Uart baudrate is %d\r\n", u.baudrate_);
}

int main(void)
{
    uart uart1;
    uart_consumer(uart1);
    uart_consumer(115200);

    return 0;
}