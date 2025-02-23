#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void init_tx_uart(void)
{
    volatile uint32_t *RCC_AHB1ENR = (uint32_t*)(0x40023800 + 0x30);
    *RCC_AHB1ENR |= (1U << 0);

    volatile uint32_t *GPIOA_MODER = (uint32_t*)(0x40020000 + 0x00);
    *GPIOA_MODER &= ~ (0b11 << 18);
    *GPIOA_MODER |= (0b10 << 18);

    volatile uint32_t *GPIOA_AFRH = (uint32_t*)(0x40020000 + 0x24);
    *GPIOA_AFRH &= ~ (0b1111 << 4);
    *GPIOA_AFRH |= (0b0111 << 4);

    volatile uint32_t *RCC_APB2ENR = (uint32_t*)(0x40023800 + 0x44);
    *RCC_APB2ENR &= ~ (1U << 4);
    *RCC_APB2ENR |= (1U << 4);

    volatile uint32_t *USART1_CR1 = (uint32_t*)(0x40011000 + 0x0C);
    *USART1_CR1 &= ~ (1U << 15);

    volatile uint32_t *USART1_BRR = (uint32_t*)(0x40011000 + 0x08);
    *USART1_BRR |= (8U << 4) | (11U << 0);

    *USART1_CR1 |= (1U << 3);
    *USART1_CR1 |= (1U << 13);
}

void transmissonEachByte(char *buf)
{
    volatile uint32_t *USART1_DR = (uint32_t*)(0x40011000 + 0x04);
    volatile uint32_t *USART1_SR = (uint32_t*)(0x40011000 + 0x00);
    while(*buf)
    {
        *USART1_DR = *buf++;
        while(!(*USART1_SR & (1U << 6)));
    }
}

void myPrintf(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    char buffer[256];
    vsnprintf(buffer,sizeof(buffer),fmt,arg);
    transmissonEachByte(buffer);
    va_end (arg);
}

int main (void)
{
    init_tx_uart();
    static float data = 3.14;
    while(1)
    {
        myPrintf("hello world: %f\n",data++);
        for(uint32_t i = 0; i < 1000000;i++ );
    }
}
