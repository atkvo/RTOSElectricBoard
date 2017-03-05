#include "LPC17xx.h"
#include "uart2driver.h"
#include "sys_config.h"

void uart2_init(void)
{
    /* 1. Enable PCONP register
     * 2. Setup PCLKSEL1 register for PCLK_UART2
     * 3. Setup baud rate (baud = PCLK)
     *      Equation: BD = PCLK / (16 * 16bit_value)
     *      Enable access to DLL/DLM registers
     *      DLL = 16bit_value & 0xFF
     *      DLM = (16bit_value >> 8) & 0xFF)
     *      Disable access to DLL/DLM registers
     *      Setup pins (PINSEL/PINMODE)
     *          Receive should NOT have pull-downs enabled
     */

    const uint32_t UART2_PCON_BIT = (1 << 24);
    const uint32_t UART2_PCLK_BY_1_BITS = (1 << 16);

    const uint8_t DLAB_BIT = (1 << 7);
    const uint32_t baudrate = 9600;

    /* Enable UART2 PCONP bit*/
    LPC_SC->PCONP |= UART2_PCON_BIT;

    /* Set UART2 PCLK divider to 1 */
    LPC_SC->PCLKSEL1 &= ~(3 << 16);
    LPC_SC->PCLKSEL1 |= UART2_PCLK_BY_1_BITS;

    /* Setup GPIO pins */
    // P2.8 (TXD3), P2.9 (RXD3)
    const uint32_t TXD2_SEL_BITS = (2 << 16);
    const uint32_t RXD2_SEL_BITS = (2 << 18);

    LPC_PINCON->PINSEL4 &= ~(3 << 24);
    LPC_PINCON->PINSEL4 |= TXD2_SEL_BITS;

    LPC_PINCON->PINSEL4 &= ~(3 << 26);
    LPC_PINCON->PINSEL4 |= RXD2_SEL_BITS;

    /* Setup Baud Rate */
    LPC_UART2->LCR = DLAB_BIT;
    uint16_t divisor = sys_get_cpu_clock()/(16 * baudrate);
    LPC_UART2->DLL = divisor & 0xFF;
    LPC_UART2->DLM = divisor >> 8;
    LPC_UART2->LCR = 0;

    /* Setup Transmit Parameters */
    // 8 E 1
    const uint8_t WORD_LEN_8 = (3 << 0);
    LPC_UART2->LCR = 0;     // Reset entire register
    LPC_UART2->LCR |= WORD_LEN_8;
}

void uart2_tx(char c)
{
    const uint8_t TRANSMIT_EMPTY_BIT = 1 << 6;
    LPC_UART2->THR = c;
    while (!(LPC_UART2->LSR & TRANSMIT_EMPTY_BIT))
    {
        ;
    }
}

uint32_t uart2_rx(char * buf, uint32_t max)
{
    const uint8_t RDR_BIT = (1 << 0);

    uint32_t i = 0;
    while((LPC_UART2->LSR & RDR_BIT) && i < max)
    {
        buf[i] = LPC_UART2->RBR;
        i++;
    }

    return i;
}
