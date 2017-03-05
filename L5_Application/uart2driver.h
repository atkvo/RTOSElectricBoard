#ifndef _UART_2_DRIVER_H_
#define _UART_2_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

void uart2_init(void);
void uart2_tx(char c);
uint32_t uart2_rx(char * buf, uint32_t max);

#ifdef __cplusplus
}
#endif
#endif