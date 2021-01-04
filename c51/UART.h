

typedef void (*UART_PROC)(unsigned char ch, void *param);

// ����1���ַ�
void UART_send_byte(unsigned char ch);

// �����ַ���
void UART_send_string(char *str);

// ��ʼ�������ж�
void UART_init(unsigned long baud, UART_PROC proc, void *param);
