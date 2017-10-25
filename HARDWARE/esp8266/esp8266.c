#include "esp8266.h"
//#include "lcd.h"
#include "all_head.h"
#include "string.h"

const char* wifista_ssid="ESP8266";		//·����ssid
const char* wifista_psw="1415926535";		//��������
const u8 ip[16]="116.196.101.77";			//Զ�˷�����ip
const u8* portnum="7788";				//����������˿ں�
u8 st;

//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//	1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)

u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	}
	return (u8*)strx;
}


//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART2_RX_STA;
//     1,����USART2_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		//printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}


//ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0;
	char *strx;
	backstage=0;
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				printf("\n%s,%d\n",USART2_RX_BUF,USART2_RX_STA&0x0FFF);delay_ms(200);
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
				strx=strstr((const char*)USART2_RX_BUF,(const char*)ack);
				if(strx)
				{			
					break;//�õ���Ч���� 
				}
				strx=strstr((const char*)USART2_RX_BUF,(const char*)"ALREAY CONNECT");
				if(strx)
				{			
					break;//�õ���Ч���� 
				}
				strx=strstr((const char*)USART2_RX_BUF,(const char*)"OK");
				if(strx)
				{			
					break;//�õ���Ч���� 
				}
				USART2_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	backstage=1;
	return res;
}


//8266��ʼ������
void esp8266_init()
{
	char str_temp[32];
	while(atk_8266_send_cmd("AT\r\n","OK",20))	//���ģ���Ƿ�����
	{
		//printf("AT+UART_CUR=115200,8,1,0,3");my_delay_ms(10);
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200); //�ر�͸��ģʽ	
		delay_ms(800);
		//printf("no fan yi\r\n");
		printf("wifi work error\n");my_delay_ms(200);
	}
	printf("wifi work regular\n");my_delay_ms(200);
	atk_8266_send_cmd("ATE0","OK",20);		//�رջ���
	delay_ms(1000);
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);  //����Ϊsta
	printf("try to connect wifi\n");my_delay_ms(200);
	sprintf((char*)str_temp,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_psw);
	while(atk_8266_send_cmd(str_temp,"WIFI GOT IP",300));	//����wifi
	printf("wifi connected\n");my_delay_ms(200);
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
	//char str_temp[32];
	//printf("try to set up TCP connect\n");my_delay_ms(200);
	//sprintf((char*)str_temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",ip,(u8*)portnum); 
	//atk_8266_send_cmd("AT+CIPMODE=0","OK",200);      //����ģʽΪ����͸��	
	//atk_8266_send_cmd(str_temp,"OK",60);
	
	//printf("TCP connect success\n");my_delay_ms(200);
}

//����TCP����
void esp8266_udppconnect()						
{
	//u8 *str_temp;
	char str_temp[32];
	printf("try to set up TCP connect\n");my_delay_ms(200);
	sprintf((char*)str_temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",ip,(u8*)portnum); 
	//atk_8266_send_cmd("AT+CIPMODE=0","OK",200);      //����ģʽΪ����͸��	
	atk_8266_send_cmd(str_temp,"OK",200);
	printf("TCP connect success\n");my_delay_ms(200);
}

//�Ͽ�UDP����
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

