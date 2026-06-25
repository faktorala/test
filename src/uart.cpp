#include "main.hpp"

// int Uart_recv;
// void Uart_Recv(void)
// {
//     printf("aaaaaaaaaaaaaaaa\n");
//     LS_UART uart(UART1, B115200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE);
//     char buf[256];
//     int temp;
//     int ret = uart.ReadData(buf, sizeof(buf));
//     if (ret < 0) {
//         printf("Reading error!\n");
//         return;
//     }
//     buf[ret] = '\0';
//     if(ret > 0){
//         temp = atoi(buf);
//         pid.Kp = temp / 100;
//         pid.Kd = temp % 100;
//     }

//     // printf("Receive %d bytes : %s\n", ret, buf);
//     ret = uart.WriteData(buf, ret);
//     if (ret < 0) {
//         printf("Writeing error!\n");
//         return;
//     }
// }

void Vofa_printf(void)
{
    LS_UART uart_vofa(UART1, B115200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE); 
    char buff[256], buf[256];
        snprintf(buff, sizeof(buff),"%.2f,%.2f\n\r",EnL,EnR);
        int ret = uart_vofa.WriteData(buff, strlen(buff));
        if(ret < 0){
            printf("Writeing error!\n");
            return;
        }
        // uart_vofa.ReadData(buf, sizeof(buf));
        // avg_error = atoi(buf);
}