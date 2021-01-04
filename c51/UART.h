

typedef void (*UART_PROC)(unsigned char ch, void *param);

// 发送1个字符
void UART_send_byte(unsigned char ch);

// 发送字符串
void UART_send_string(char *str);

// 初始化串口中断
void UART_init(unsigned long baud, UART_PROC proc, void *param);
