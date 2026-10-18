#include <cstdint>
#include <cstdio>

class uart {
    public:
        uart(std::uint32_t baud = 9600): baudrate_(baud){}
        void init()
        {
            printf("Init uart, with baudrate %d\r\n", baudrate_);
        }

    private:
        std::uint32_t baudrate_;
};

class sensor{
    public:
        sensor(uart &u, std::uint32_t read_interval):
                uart_(u),
                read_interval_(read_interval)
        {
            u.init();
            printf("Init sensor\r\n");
        };
    private:
        uart &uart_;
        const std::uint32_t read_interval_;
};

int main(void)
{
    uart uart1;
    sensor sensor1(uart1, 500);
    return 0;
}
