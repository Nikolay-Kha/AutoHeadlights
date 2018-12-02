#include "debug.h"
#include "stm8s_uart1.h"
#include "stm8s_gpio.h"
#include <stdarg.h>

#ifdef DEBUG
void debug_init() {
	UART1_DeInit();
	UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
            UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
	UART1_Cmd(ENABLE);
}

void uart_send(const char c) {
    UART1_SendData8(c);
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

void vprintf(const char *pFormat,  va_list ap) {
	char digitBuffer[32];
	char pf;

	/* Phase string */
	while ( (pf = *pFormat) != 0){
		if(pf != '%'){
			uart_send(pf);
			pFormat++;
		} else if(pFormat[1] == '%'){
			uart_send('%');
			pFormat += 2;
		} else {
			pFormat++;
			pf = *pFormat;

			switch (pf) {
			case 'd':
			case 'i':{
				char *tmp = digitBuffer;
				int val = va_arg(ap, signed int);
				if(val < 0) {
					val = -val;
					uart_send('-');
				}
				do {
					*tmp++ = val % 10 + '0';
				} while ( (val /= 10) > 0);
				tmp--;
				while(tmp >= digitBuffer) uart_send(*tmp--);
			}
			break;
			case 'u':
			case 'x':
			case 'X':
			{
				unsigned int base = (pf == 'u') ? 10 : 16;
				char *tmp = digitBuffer;
				unsigned int val = va_arg(ap, unsigned int);
				do {
					unsigned int c = val % base;
					if(c < 10)
						*tmp++ = c + '0';
					else if(pf == 'x')
						*tmp++ = c - 10 + 'a';
					else
						*tmp++ = c - 10 + 'A';
				} while ( (val /= base) > 0);
				tmp--;
				while(tmp >= digitBuffer) uart_send(*tmp--);
			}
			break;
			case 's': {
				char * tmp = va_arg(ap, char *);
				char c;
				while ((c = *tmp++)) uart_send(c);
			}
			break;
			case 'c': uart_send(va_arg(ap, unsigned int)); break;
			default:
				uart_send('%');
				uart_send(pf);
				break;
			}
			pFormat++;
		}
	}
}

void printf(const char *pFormat, ...) {
    va_list    ap;
    va_start(ap, pFormat);
    vprintf(pFormat, ap);
    va_end(ap);
}

void debug(const char *pFormat, ...) {
    va_list    ap;
    va_start(ap, pFormat);
    vprintf(pFormat, ap);
    va_end(ap);
    uart_send('\r');
    uart_send('\n');
}

#endif //DEBUG
