#include "gprs.h"
#include "all_head.h"

const char* ip_num="139.199.172.253";//服务器IP
const char* port_num="10010";	//服务器程序端口号
unsigned char SEND_READY=0;

/******************************************
	函数功能:初始化GPRS
******************************************/
void gprs_init(void)
{
	//char str_temp[32];
	if(atk_A7_send_cmd("AT+RST\r\n","OK",100))
	{
			atk_A7_send_cmd("AT+RST\r\n","OK",100);
	}
	
	/**  检测SIM卡是否准备好 **/
	if(atk_A7_send_cmd("AT+CPIN?\r\n","READY",100)==1)
	{
			atk_A7_send_cmd("AT+CPIN?\r\n","READY",100);
	}
	
	if(atk_A7_send_cmd("AT+CREG?\r\n","CREG: 1",100)==0)
	{
			atk_A7_send_cmd("AT+CREG?\r\n","CREG: 1",100);
	}

	/*****设置GPRS模块*****/
	atk_A7_send_cmd("AT+CGATT=1\r\n","OK",100);
	atk_A7_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK",100);
	atk_A7_send_cmd("AT+CGACT=1,1\r\n","OK",100);
	
	TIM3_Int_Init(39999,7199); //10Khz的计数频率，计数到40000为4s 
}
//A7模块发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_A7_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	}
	return (u8*)strx;
}

//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
void atk_A7_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}


//A7发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
char atk_A7_send_cmd(char *cmd,char *ack,u16 waittime)
{
	u8 res=0;
	char *strx;
	//backstage=0;
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			my_delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				//printf("\n%s,%d\n",USART2_RX_BUF,USART2_RX_STA&0x0FFF);
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
				strx=strstr((const char*)USART2_RX_BUF,(const char*)ack);
				if(strx)
				{
					//return 0;
					break;//得到有效数据 
				}
				USART2_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	//backstage=1;
	return res;
}
/****************************************************
	函数功能:发送TCP数据
		失败返回0
		成功返回1
****************************************************/
unsigned char send_TCP_message(char *p)
{
		char ch[3];
		char send_buf[100] = {0};
		//LED=!LED;
		ch[0]=0x1a;ch[1]='\r';ch[2]='\n';
		memset(send_buf, 0, 100);    //清空
		strcpy(send_buf, "AT+CIPSTART=\"TCP\",\"");
		strcat(send_buf, ip_num);
		strcat(send_buf, "\",");
		strcat(send_buf, port_num);
		strcat(send_buf, "\r\n");
		if(atk_A7_send_cmd(send_buf,"OK",400))
		{
			printf("connet failed?\n");my_delay_ms(5);
			atk_A7_send_cmd(send_buf,"OK",200);
		}
		
		u2_printf("AT+CIPSEND\r\n");
		my_delay_ms(40);
		atk_A7_send_cmd(p,p,20);
		atk_A7_send_cmd(ch,ch,20);
		my_delay_ms(20);
		atk_A7_send_cmd("AT+CIPCLOSE\r\n", "OK",40);
		return 1;
}
char close_TCP_connet(void)
{
	if(atk_A7_send_cmd("AT+CIPCLOSE\r\n", "OK",100)){
		return 1;
	}
	else{
		return 0;
	}
}






