#include <cstdint>
#include <cstdio>

class uart {
    public:
        uart(std::uint32_t baud = 9600): baudrate_(baud){}
        void init()
        {
            write_brr(calculate_uartdiv());
        }

    private:
        std::uint32_t baudrate_;
        std::uint32_t calculate_uartdiv()
        {
            printf("Calculate Uartdiv (baud: %d)\n\r", baudrate_);
            return baudrate_ / 32000;
        }

        void write_brr(std::uint8_t)
        {
            printf("Write brr\n\r");
        }
};

int main(void)
{
    uart uart1(115200);
    uart1.init();

    return 0;
}
