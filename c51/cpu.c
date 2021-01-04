//头文件:
#include <reg52.h>
#include <stdio.h>
#include "uart.h"    // 串口


// IO引脚定义
sbit DATA_IN  = P1^0;
sbit DATA_LD  = P1^1;
sbit DATA_CLK = P1^2;
sbit IN_CLK   = P1^3;

// 延时子函数,int为2个字节,long为4个字节
// 参数 time 大小决定延时时间长短
void delay(unsigned long n)
{
	n *= 10;

	while(n--); // 用于延时
}

void uart_proc(unsigned char ch, void *param)
{
    UART_send_byte(ch);
}

void save_data()
{
    DATA_LD = 0;    // save data
    delay(1);
    DATA_LD = 1;
    delay(1);
}

unsigned long get_data(unsigned int count)
{
    unsigned int i;
    unsigned long d;
    unsigned long n = 0;

    for (i = 0; i < count; i++)
    {
        d = DATA_IN;    // read data
        delay(1);
        DATA_CLK = 0;
        delay(1);
        DATA_CLK = 1;   // output data
        delay(1);
        
        n |= (d << i);
    }

    return n;
}

void get_mi()
{
    char buff[32];
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int l;

    save_data();
    UART_send_string("--save--data--\r");
    
    i = get_data(12);
    sprintf(buff, "MINA:    0x%03X\r", i);

    UART_send_string(buff);
    i = get_data(12);
    sprintf(buff, "MIA:     0x%03X\r", i);
    UART_send_string(buff);

    i = get_data(12);
    sprintf(buff, "MIN:     0x%03X\r", i);
    UART_send_string(buff);

    i = get_data(8);
    sprintf(buff, "A:       0x%02X\r", i);
    UART_send_string(buff);

    i = get_data(8);
    sprintf(buff, "D:       0x%02X\r", i);
    UART_send_string(buff);

    i = get_data(8);
    sprintf(buff, "F:       0x%02X\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "SIG-JL:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "SIG-JNE: %d\r", i);
    UART_send_string(buff);

    i = get_data(2);
    sprintf(buff, "SEL-AB:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-IR:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-INT: %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JE:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JNE: %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JB:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JBE: %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JL:  %d\r", i);
    UART_send_string(buff);

    i = get_data(1);
    sprintf(buff, "CHK-JLE: %d\r", i);
    UART_send_string(buff);

    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "IR:%s %s\r", (j==0?"R":"W"), (k==0?"DBus":""));
    UART_send_string(buff);

    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "IF:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);

    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "IP:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);

    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "SP:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);

    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "AR:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);


    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "BR:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);

    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "CR:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);


    i = get_data(1);
    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "DR:%s %s %s\r", (j==0?"R":"W"), (k==0?"DBus":""), (i==0?"ABus":""));
    UART_send_string(buff);

    i = get_data(6);
    if (i == 0) sprintf(buff, "ALU:++\r");
    else if (i == 0x34) sprintf(buff, "ALU:0xFF\r", i);
    else sprintf(buff, "ALU:%x\r", i);
    UART_send_string(buff);

    j = get_data(1);
    k = get_data(1);
    sprintf(buff, "TR:%s %s\r", (j==0?"R":"W"), (k==0?"DBus":""));
    UART_send_string(buff);

    i = get_data(1); // M-CE
    j = get_data(1); // M-RW
    k = get_data(1); // M-OE
    l = get_data(1); // M-WE
    sprintf(buff, "MEM:%s OE:%d WE:%d\r", (i==0?(j==0?"R":"W"):""), j, k);
    UART_send_string(buff);

    i = get_data(1); // SIG-ROM
    sprintf(buff, "SIG-ROM:%d\r", i);
    UART_send_string(buff);

    i = get_data(1); // SIG-RAM
    sprintf(buff, "SIG-RAM:%d\r", i);
    UART_send_string(buff);
}

//主函数，C语言的入口函数
void main(void)
{
    unsigned int last = 1;

    UART_init(2400, uart_proc, '\n');

    while (1)
	{
        if (last == 1 && IN_CLK == 0)
        {
             get_mi();
        }

        last = IN_CLK;

        delay(10);
	}
}
