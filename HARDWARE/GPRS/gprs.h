#ifndef __GPRS_H
#define __GPRS_H

#include "all_head.h"

extern unsigned char SEND_READY;

void gprs_init(void);
u8* atk_A7_check_cmd(u8 *str);
void atk_A7_at_response(u8 mode);
u8 atk_A7_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
unsigned char send_TCP_message(char *p);
char close_TCP_connet(void);

#endif

