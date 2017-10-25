#include "esp8266.h"
//#include "lcd.h"
#include "all_head.h"
#include "string.h"

const char* wifista_ssid="ESP8266";		//路由器ssid
const char* wifista_psw="1415926535";		//连接密码
const u8 ip[16]="116.196.101.77";			//远端服务器ip
const u8* portnum="7788";				//服务器程序端口号
u8 st;

//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//	1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}
//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)

u8* atk_8266_check_cmd(u8 *str)
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
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		//printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}


//ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0;
	char *strx;
	backstage=0;
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				printf("\n%s,%d\n",USART2_RX_BUF,USART2_RX_STA&0x0FFF);delay_ms(200);
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
				strx=strstr((const char*)USART2_RX_BUF,(const char*)ack);
				if(strx)
				{			
					break;//得到有效数据 
				}
				strx=strstr((const char*)USART2_RX_BUF,(const char*)"ALREAY CONNECT");
				if(strx)
				{			
					break;//得到有效数据 
				}
				strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
				if(strx)
				{			
					break;//得到有效数据 
				}
				USART2_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	backstage=1;
	return res;
}


//8266初始化函数
void esp8266_init()
{
	char str_temp[32];
	while(atk_8266_send_cmd("AT\r\n","OK",20))	//检测模块是否在线
	{
		//printf("AT+UART_CUR=115200,8,1,0,3");my_delay_ms(10);
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200); //关闭透传模式	
		delay_ms(800);
		//printf("no fan yi\r\n");
		printf("wifi work error\n");my_delay_ms(200);
	}
	printf("wifi work regular\n");my_delay_ms(200);
	atk_8266_send_cmd("ATE0","OK",20);		//关闭回显
	delay_ms(1000);
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);  //设置为sta
	printf("try to connect wifi\n");my_delay_ms(200);
	sprintf((char*)str_temp,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_psw);
	while(atk_8266_send_cmd(str_temp,"WIFI GOT IP",300));	//连接wifi
	printf("wifi connected\n");my_delay_ms(200);
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
	//char str_temp[32];
	//printf("try to set up TCP connect\n");my_delay_ms(200);
	//sprintf((char*)str_temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",ip,(u8*)portnum); 
	//atk_8266_send_cmd("AT+CIPMODE=0","OK",200);      //传输模式为：非透传	
	//atk_8266_send_cmd(str_temp,"OK",60);
	
	//printf("TCP connect success\n");my_delay_ms(200);
}

//建立TCP连接
void esp8266_udppconnect()						
{
	//u8 *str_temp;
	char str_temp[32];
	printf("try to set up TCP connect\n");my_delay_ms(200);
	sprintf((char*)str_temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",ip,(u8*)portnum); 
	//atk_8266_send_cmd("AT+CIPMODE=0","OK",200);      //传输模式为：非透传	
	atk_8266_send_cmd(str_temp,"OK",200);
	printf("TCP connect success\n");my_delay_ms(200);
}

//断开UDP连接
void esp8266_udpclose()						
{
	atk_8266_send_cmd("AT+CIPCLOSE","OK",30);
	st=0;
	//printf("tcpclosed already!\r\n");
}


void upload(u8 *str)
{
	int num=strlen(str);
	char strte[20];
	char str_temp[32];
	//printf("try to set up TCP connect\n");my_delay_ms(200);
	sprintf((char*)str_temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",ip,(u8*)portnum); 
	atk_8266_send_cmd(str_temp,"OK",200);
	//printf("TCP connect success\n");my_delay_ms(200);
	sprintf(strte,"AT+CIPSEND=%d\r\n",num);
	atk_8266_send_cmd(strte,">",60);
	USART2_RX_STA=0;
	//delay_ms(10);
	u2_printf("%s",str);
	atk_8266_send_cmd("AT+CIPCLOSE","OK",30);
}

