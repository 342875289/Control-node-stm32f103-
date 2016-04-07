#include "esp8266.h"
#define INITFAIL {LCD_Print(0,6,"WIFI-��ʼ��-ʧ�� ");LCD_Print(105,0,"NO");}

int is_con_slave=0;//����Ƿ����ӵ��ӻ�
int rn_count = 0;//�Խ�����Ϣ�еĻ��з��������յ�3��\r\n��Ϊ��������
char wifi_feedback[200];//���ڴ�Ž������ݵ�����
int data_i=0;//�����������������
int time_wait=3;//�������ݵȴ���ʱ��(3��)
int receive_state=0;//Ϊ0��ʾ��û���յ�����  Ϊ1��ʾ���ڽ������� Ϊ2��ʾ�������ݵĹ�����������
u8 fac_us=72000000/8000000;	
u16 fac_ms=72000000/8000000*1000;
	
void Net_Init()
{
	LCD_Print(105,0,"AT");//�����ʼ��ģʽ
	LCD_Print(0,6,"��ʼ��WIFIģ���� ");//������ʾ9�����ֻ���16������ַ�
	
	//�ȴ�ģ�������������
	delay_ms(1000);
	
	delay_ms(1000);
	delay_ms(1000);
	
	
	//�ر��������
	printf("ATE0\r\n");
	if(Re(10)==1){LCD_Print(0,6,"Close--show---OK");}
	else{LCD_Print(0,6,"WiFi-Init--ERROR");INITFAIL return ;}
	delay_ms(1000);
	
	
	//��Ϊ��������WiFi�ȵ�
	LCD_Print(0,6,"��������WiFi...  ");
	
	printf("AT+CWJAP=\"Mon-PC\",\"317000000\"\r\n");
	
	if(Re(10)==1){LCD_Print(0,6,"WiFi--Access--OK");}
	else{LCD_Print(0,6,"WIFI-------ERROR");INITFAIL return ;}
	delay_ms(1000);
	
		
	//��Ϊ����-����AP�ȵ㹩�ӻ�����
	printf("AT+CWSAP=\"lift-project\",\"labcat127\",1,4,4\r\n");
	
	if(Re(10)==1){LCD_Print(0,6,"Ad hoc--Init--OK");}
	else{LCD_Print(0,6,"Adhoc-Init-ERROR");INITFAIL return ;}
	delay_ms(1000);

	//��������APģʽ�µ�IP
	printf("AT+CIPAP=\"192.168.4.1\"\r\n");
	
	if(Re(10)==1){LCD_Print(0,6,"Ad hoc--Init--OK");}
	else{LCD_Print(0,6,"Adhoc-Init-ERROR");INITFAIL return ;}
	delay_ms(1000);

	//��������Ϊ������
	printf("AT+CIPMUX=1\r\n");
	
	if(Re(10)==1){LCD_Print(0,6,"Ad hoc--Init--OK");}
	else{LCD_Print(0,6,"Adhoc-Init-ERROR");INITFAIL return ;}
	delay_ms(1000);
	

	
	//��������������ģʽ���˿�Ϊ8088
	printf("AT+CIPSERVER=1,8088\r\n");
	
	if(Re(10)==1){LCD_Print(0,6,"Server--Init--OK");}
	else{LCD_Print(0,6,"Server-Init-ERROR");INITFAIL return ;}
	delay_ms(1000);
		
	//������SensorWeb������������
	printf("AT+PING=\"115.28.147.177\"\r\n");
	if(Re(10)==1){LCD_Print(0,6,"Ping--Server--OK");}
	else{LCD_Print(0,6,"PingServer-ERROR");INITFAIL return ;}
	delay_ms(1000);
	
	//���������������
	printf("AT+CIPSTART=0,\"TCP\",\"115.28.147.177\",80\r\n");
	if(Re(10)==1){LCD_Print(0,6,"Connect-Sever-OK");}
	else{LCD_Print(0,6,"Con-Sever--ERROR");return ;}
	delay_ms(1000);
	
	
	
	/*
	//�������ݲ���
	
	//���������������
	printf("AT+CIPSTART=0,\"TCP\",\"115.28.147.177\",80\r\n");
	if(Re(10)==1){LCD_Print(0,6,"Connect-Sever-OK");}
	else{LCD_Print(0,6,"Con-Sever--ERROR");return ;}
	delay_ms(1000);
	
	//��������
	LCD_Print(0,6,"Send-Data--READY");
	printf("AT+CIPSEND=0,205\r\n");
	if(Re(10)==1){LCD_Print(0,6,"Send--Ready---OK");}
	else{LCD_Print(0,6,"Send-Ready-ERROR");}
	delay_ms(1000);
	
	LCD_Print(0,6,"Send-Data----ing");
	printf("POST /server/submit.php HTTP/1.1\r\n");
	printf("Host: 115.28.147.177\r\n");
	printf("Content-Length: 78\r\n");
	printf("Content-Type: application/x-www-form-urlencoded\r\n");
	printf("\r\n");
	printf("sensor_id=0001&acceleration=1.897&speed=1.462&frequence=54.364&voltage=218.386\r\n");
	
	
	if(Re(10)==1)//ȷ���Ƿ�ɹ�����
	{	
		if(Re(10)==1){LCD_Print(0,6,"Send-Data-OK-ACK");}
		else{LCD_Print(0,6,"Send-Data-NO-ACK");}
	}
	else{LCD_Print(0,6,"Send-Data--ERROR");return;}
	delay_ms(1000);
	
	*/
	
	
	LCD_Print(105,0,"OK");//WiFiģ���ʼ����ɣ�����ȫ���ɹ�
	LCD_Print(0,6,"WIFI - ��ʼ���ɹ�");
	
	
	//�������ڽ����ж�--���մӻ���������Ϣ
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	
	//�Ͽ�TCP/IP����
	//printf("AT+CIPCLOSE");
	
	
	//ReceiveData();
	//LCD_Print(0,6,wifi_feedback);
	/*while(1)
	{
		net_flag++;
	delay_ms(1000);
	}*/
	
	//if(strncmp("OK","OK",2) == 0) {net_flag=1;}
	
	//printf("AT+CWJAP=\"Mon-PC\",\"317000000\"\r\n");
	//ReceiveData();
	
	
	//printf("AT+CWQAP\r\n");
	//ReceiveData();
	//����WIFI
	//printf("AT+CWJAP=\"Mon-PC\",\"317000000\"\r\n");
	//���� WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n
	
	//�Ͽ�WIFI����
	//printf("AT+CWQAP\r\n");
	//���� ---�ɹ��˳� \r\nOK\r\nWIFI DISCONNECT\r\n
	//     ---�Ƴ�ǰ����������  \r\nOK\r\n\r\n
	
	//��ѯ�Ƿ�������WIFI
	//printf("AT+CWJAP?");
	//���� ---������  +CWJAP:"Mon-PC","46:fd:52:3f:f8:8c",1,-49\r\n\r\nOK\r\n
	//     ---������  No AP\r\n\r\nOK\r\n
	
	//������Զ�̷�����������(�����Ƿ��н�������)
	//printf("AT+PING=\"115.28.147.177\"");
	//���� ---������  +45\r\n\r\nOK\r\n   (+45Ϊ�ӳ٣���С��һ��ÿ��һ��)
	//     ---������  +timeout\r\n\r\nERROR\r\n
	
	//������SensorWeb������
	//printf("AT+CIPSTART=\"TCP\",\"115.28.147.177\",80");
	//���� ---�ɹ�  CONNECT\r\n\r\nOK\r\n
	//     ---ʧ��  no ip\r\n\r\nERROR\r\n
	
	//�Ͽ�TCP/IP����
	//printf("AT+CIPCLOSE");
	//���� ---�ɹ�  CLOSED\r\n\r\nOK\r\n
	//     ---������  \r\nERROR\r\n

	 
	/*
	LCD_Print(0,0,"���:");
		LCD_Print(35,0,"ֹͣ");
		intTo3chars(U,u_chars);
		floatTo5chars(sinFre,fre_chars);
		LCD_Print(0,2,"��ѹ:");
		LCD_Print(36,2,u_chars);
		LCD_Print(62,2,"Ƶ��:");
		LCD_Print(96,2,fre_chars);
		
		//���������ʾ
		intTo3chars(impulse_num,num_chars);
		intTo3chars(impulse_step,step_chars);
		LCD_Print(0,4,"������");
		LCD_Print(43,4,num_chars);
		LCD_Print(72,4,"����");
		LCD_Print(100,4,step_chars);
		
		//�������Ӳ���
		LCD_Print(68,0,"����:");
		LCD_Print(105,0,"��");
		LCD_Print(0,6,"���");
		LCD_Print(30,6,"WiFi");
		LCD_Print(60,6,"ģ����");
		*/
		
}

int Re(int seconds)
{
	u32 temp;
	u8 data;
	u16 nms=1000;
	int times=seconds;
	data_i=0;
	
	while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET );//�ȴ���һ���ֽ�
	
	while(times--)
	{
		SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
		SysTick->VAL =0x00;							//��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
		do
		{
			if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
			{
				data = USART_ReceiveData(USART3);
				wifi_feedback[data_i]=data ;
				//OK
				if( wifi_feedback[data_i] == 'K' && data_i>=1 && 
					wifi_feedback[data_i-1] == 'O' )
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
					SysTick->VAL =0X00;       					//��ռ�����
					//���سɹ���Ϣ
					return 1;
				}
				//ERROR
				if( wifi_feedback[data_i] == 'R' && data_i>=4 && 
					wifi_feedback[data_i-1] == 'O' &&
					wifi_feedback[data_i-2] == 'R' &&
					wifi_feedback[data_i-3] == 'R' &&
					wifi_feedback[data_i-4] == 'E' 
				)
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
					SysTick->VAL =0X00;       					//��ռ�����
					//����ʧ����Ϣ
					return 0;
				}
				
				//���ճɹ�-����װ�ص���ʱ����
				times=seconds;
				data_i++;
			}
			
			
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
		SysTick->VAL =0X00;       					//��ռ�����	
	}
	return 0;
}


void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u32 temp;
	u8 data;
	u16 nms=1000;
	int times=10;
	data_i=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//ʧ�ܽ����ж�--׼�����������ַ�
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		while(times--)
		{
			SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
			SysTick->VAL =0x00;							//��ռ�����
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
			do
			{
				if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
				{
					data = USART_ReceiveData(USART3);
					wifi_feedback[data_i]=data ;
					//CONNECT
					if( wifi_feedback[data_i] == 'T' && data_i>=6 && 
						wifi_feedback[data_i-1] == 'C' &&
						wifi_feedback[data_i-2] == 'E' &&
						wifi_feedback[data_i-3] == 'N' &&
						wifi_feedback[data_i-4] == 'N' &&
						wifi_feedback[data_i-5] == 'O' &&
						wifi_feedback[data_i-6] == 'C' )
					{
						SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
						SysTick->VAL =0X00;       					//��ռ�����	
						//���سɹ���Ϣ
						is_con_slave=1;
						LCD_Print(0,6,"WIFI-�ӻ����ӳɹ�");
						return ;
					}
					//ERROR
					if( wifi_feedback[data_i] == 'R' && data_i>=4 && 
						wifi_feedback[data_i-1] == 'O' &&
						wifi_feedback[data_i-2] == 'R' &&
						wifi_feedback[data_i-3] == 'R' &&
						wifi_feedback[data_i-4] == 'E' 
					)
					{
						SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
						SysTick->VAL =0X00;       					//��ռ�����	
						//����ʧ����Ϣ
						is_con_slave=0;
						LCD_Print(0,6,"WIFI-�ӻ�����ʧ��");
						//���Ӵӻ�ʧ��--����ʹ�ܽ����ж�--׼���´�����
						USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
						return ;
					}
					
					//���ճɹ�-����װ�ص���ʱ����
					times=10;
					data_i++;
				}
				
				
				temp=SysTick->CTRL;
			}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
			SysTick->VAL =0X00;       					//��ռ�����	
		}
		
     } 
	is_con_slave=0;
	//���Ӵӻ�ʧ��--����ʹ�ܽ����ж�--׼���´�����
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
} 

int Receive()
{
	time_wait=6;
	//receive_state=0;
	data_i=0;
	rn_count = 0;
	while(receive_state==0);//�ȴ����ݷ���
	while(receive_state==1)//�ӽ��յ�һ���ֽڵ����ݿ�ʼ������6��������Ϊ����ʧ��
	{
		delay_ms(1000);
		time_wait--;
		if(time_wait==0){return 0;}
		if(receive_state==2){receive_state=0;return 1;}
	}
	return 1;
}

