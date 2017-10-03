#include "gprs.h"
#include "all_head.h"

const char* ip_num="139.199.172.253";//������IP
const char* port_num="10010";	//����������˿ں�
unsigned char SEND_READY=0;

/******************************************
	��������:��ʼ��GPRS
******************************************/
void gprs_init(void)
{
	//char str_temp[32];
	if(atk_A7_send_cmd("AT+RST\r\n","OK",100))
	{
			atk_A7_send_cmd("AT+RST\r\n","OK",100);
	}
	
	/**  ���SIM���Ƿ�׼���� **/
	if(atk_A7_send_cmd("AT+CPIN?\r\n","READY",100)==1)
	{
			atk_A7_send_cmd("AT+CPIN?\r\n","READY",100);
	}
	
	if(atk_A7_send_cmd("AT+CREG?\r\n","CREG: 1",100)==0)
	{
			atk_A7_send_cmd("AT+CREG?\r\n","CREG: 1",100);
	}

	/*****����GPRSģ��*****/
	atk_A7_send_cmd("AT+CGATT=1\r\n","OK",100);
	atk_A7_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK",100);
	atk_A7_send_cmd("AT+CGACT=1,1\r\n","OK",100);
	
	TIM3_Int_Init(39999,7199); //10Khz�ļ���Ƶ�ʣ�������40000Ϊ4s 
}
//A7ģ�鷢�������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_A7_check_cmd(u8 *str)
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
void atk_A7_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}


//A7��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
char atk_A7_send_cmd(char *cmd,char *ack,u16 waittime)
{
	u8 res=0;
	char *strx;
	//backstage=0;
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			my_delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				//printf("\n%s,%d\n",USART2_RX_BUF,USART2_RX_STA&0x0FFF);
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
				strx=strstr((const char*)USART2_RX_BUF,(const char*)ack);
				if(strx)
				{
					//return 0;
					break;//�õ���Ч���� 
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
	��������:����TCP����
		ʧ�ܷ���0
		�ɹ�����1
****************************************************/
unsigned char send_TCP_message(char *p)
{
		char ch[3];
		char send_buf[100] = {0};
		//LED=!LED;
		ch[0]=0x1a;ch[1]='\r';ch[2]='\n';
		memset(send_buf, 0, 100);    //���
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






