#include <span>
#include <cstdio>
#include <cstdint>

class uart {
    public:
        virtual void init(std::uint32_t baudrate) = 0;
        virtual void write(std::span<const char> data) = 0;
};

class uart_stm32 : public uart{
    public:
    void init(std::uint32_t baudrate = 9600) override
    {
        printf("uart_stm32::init: setting baudrate to %d\r\n", baudrate);
    }

    void write(std::span<const char> data) override
    {
        printf("uart_stm32::write: ");
        for(auto ch: data)
        {
            putc(ch, stdout);
        }
    }
};

class gsm_lib{
    public:
        gsm_lib(uart &u) : uart_(u) {}

        void init()
        {
            printf("gsm_lib::init: sending AT command\r\n");
            uart_.write("AT");
        }
    private:
        uart &uart_;

};

int main(void)
{
    uart_stm32 uart_stm32_obj;
    uart_stm32_obj.init(115200);

    gsm_lib gsm(uart_stm32_obj);
    gsm.init();

    return 0;
}