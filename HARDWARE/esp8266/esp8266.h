#include "sys.h"
#include "usart2.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
//#include "malloc.h"


extern u8 st;


void esp8266_init(void);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* atk_8266_check_cmd(u8 *str);
void atk_8266_at_response(u8 mode);
void esp8266_udppconnect(void);
void esp8266_udpclose(void);
void upload(u8 *str);
u8 atk_8266_quit_trans(void);
