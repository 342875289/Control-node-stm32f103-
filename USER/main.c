#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "math.h"
#include "oled.h"
#include "esp8266.h"

/*
���߼�¼��
PA.8  (CH1)   PA.9 (CH2)   Tim1 SPWM  ����� (�����ͬ)
PA.10 (CH3)  PA.11 (CH4)   Tim1 SPWM  ����� (�����ͬ)

PA.8  (CH1)		-����		-�� 		 (-��)
PA.9  (CH2)		-����		-��			 (-��)

PA.10 (CH3)		-����		-��		     (-��)
PA.11 (CH4)		-����		-��			 (-��)

���ذ����� Up-��    Down-��  (Ĭ��Ϊ��)
������̣� A<->F-  3<->F+   2<->U-  1<->U+      ������һ���ڽ�PC0 ����7����(����)��PE2����ɫ�ߣ�~PF6����ɫ�ߣ�
OLED VCC-3.3V GND  D0(PF12)~DS(PB13) (4��������)
************************************************/

//�����趨��
#define Pi 3.14159265
#define fre 5000 //�ز�Ƶ��
#define tim_Prescaler 2 //��Ƶϵ�� ��Ҫͬʱ�����������ߵ�ֵ����ʹ�ز�Ƶ�ʵı仯��Ч
#define time(x,f) (3600*x*f/1000000)
#define deadtime  (time(20,fre))
#define transformer2 300 //�����ѹ�����αߵ�ѹ
//����ʵ�� 1Khz 2Khz 5Khz
/***
 CCR=3600 ��Ϊ0us
 CCR=0    ��Ϊ1000us
 ��ÿ1		��Ϊ 1000 / 3600 = 0.2778 us
 �� 100us<--> 360
***/
void calculetaTable(float frequence,int V);

//���Ʋ���
float sinFre=50.0;//���Ҳ�Ƶ��50Hz
int N;//���Ҳ������ڲ�����
int U=220;//���Ҳ���ѹ����
float a;//���Ʊ�

//��¼����״̬�ı�־
int outputstate=0;//����Ƿ����������ģʽ
int is_impulse=0;//����Ƿ��������ģʽ
int impulse_i=0;//������� 1����λΪһ�����Ҳ����� ��50��Ϊ����һ�������
int impulse_num=25;//������� 1����λΪһ�����Ҳ����� ��50��Ϊ����һ�������
int impulse_step=5;//���������������

//���SPWMռ�ձȵ�����
unsigned int sintable_OC1[400],sintable_OC1_backup[400];
unsigned int sintable_OC2[400],sintable_OC2_backup[400];
char isUseBackup=0,flag_newdata=0;
int show_key;
short az;
int main(void)
{	
	int i;//����ѭ������
//	char keychar[4];//��ʾ������λ
	u8 key=0;//���水����λ
	//��ʼ�����������   ֻ��ʼ�����°�������  ���Ұ�����������
	KEY_Init();
	//��ʼ���������
	MatrixKey_Init();
	//��ʱ������ʼ��	
	delay_init();
//	LCD_All_Init(); //��ʼ��LCD��
	//��ʼ��OLED��
	OLED_Init();	
	//��ʾ��ǰ����
	showdata_init();
	//����NVIC�жϷ���2 : 2λ��ռ���ȼ�(�ж�Ƕ�״���)��2λ��Ӧ���ȼ�(�ж���Ӧ�Ⱥ�˳��)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ�� ����USART3  ������921600 8λ����λ 1λֹͣλ 0λУ��λ
	UART_Init(921600);
	
	
	//��ʼ��WIFIģ����������--����ͨ������B���³�ʼ��
	//Net_Init();  
	
	//1000ms��ʱ����ȡ����������
	TIM3_Int_Init(30000,7200);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	//��ʼ�� TIM1 SPWM���  ����Ƶ  �ز�Ƶ�� 5 Khz 
	TIM1_SPWM_Init(tim_Prescaler);
	for(i=0;i< 400 ;i++)//����ɾ�������ڰѱ���Ϊȫ0ռ�ձ�
	{
		sintable_OC1[i]=sintable_OC1_backup[i]=3600;
		sintable_OC2[i]=sintable_OC2_backup[i]=3600;
	}
	//����sin��
	calculetaTable(sinFre,U);
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	
	//printf("START\r\n");
	
	while(1)
	 {
		 key=KEY_Scan(0);	//�õ���ֵ
				if(key)
			{						   
				switch(key)
				{	
					//Key0  ��
					//case 1:	 break;
					case 2:	//Key1  ��
						//U+=5;
					TIM_CtrlPWMOutputs(TIM1, DISABLE);
					TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE ); 
					outputstate=0;
					LCD_Print(35,0,"ֹͣ");
						break;
					//Key2   ��
					//case 3:break;	
						
					case 4:	//Key_Up ��
					TIM_CtrlPWMOutputs(TIM1, ENABLE);
					TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); 
					outputstate=1;
					LCD_Print(35,0,"����");
						break;
				}
		}
			
			
			key = MatrixKey_Scan();
			if(key !=0)//LCD_ShowNum(250,450,key,2,24);
			{		
				show_key=key;				
				keyfunc(key);		
				calculetaTable(sinFre,U);
				showdata();
				
				/*
				//��ʾ����ֵ
				intTo3chars(key,keychar);
				LCD_Print(100,6,keychar);
				*/
				
			}
			else delay_ms(10); 
						
	 }
	 
	
	
 }
int times=0;
 

void calculetaTable(float frequence,int V)
{
	int i=0;//,N_new;
		unsigned int temp;
	//N_new = (int)(10000/frequence);
	//if(N_new != N )
	//{
		  //N  = N_new;
		  //V = 2*frequence;
    N = (int)(fre/frequence);
		a = (float)V / transformer2 ;
		
		if(isUseBackup==0)
		{
			if(N%2==1)//Ҫ����ĵ㣬����Ϊ����
			{}
				for(i=0;i< N/2;i++)//����sin���ݱ�ռ�ձȱ仯����
				{ 
					temp = (unsigned int)(a*sin(2*Pi/N *(i+1))*3600);
					if(temp < deadtime &&  temp!=0 ){temp = deadtime;}
					else if(temp > (3600 - deadtime)){temp = 3600;}
					
					sintable_OC1_backup[i]     = 3600  - temp;
					sintable_OC2_backup[N/2+i] = sintable_OC1_backup[i];
					
				//50hz����(1)
				//sintable_OC1_backup[i]     = 0;
				//sintable_OC2_backup[N/2+i] = sintable_OC1_backup[i];
				
				}
				/*
				//50hz����(2)
					sintable_OC1_backup[0]     = 3600;
				sintable_OC2_backup[N/2] = sintable_OC1_backup[0];
				i--;
				sintable_OC1_backup[i]     = 3600;
				sintable_OC2_backup[N/2+i] = sintable_OC1_backup[i];
				i=N-2;
				*/
				
				/*
				//������Ϊ��ȷ��sin��������ʱռ�ձ�Ϊ0
				sintable_OC1_backup[i]     = 3600;
				sintable_OC2_backup[N/2+i] = 3600;
				i++;
				*/
				for(;i< N ;i++)//����sin���ݱ�Ϊ0����
				{ 
					sintable_OC1_backup[i]     = 3600 ;
					sintable_OC2_backup[i-N/2] = 3600;
				}
				
			}
			else
			{
				for(i=0;i< N/2 ;i++)//����sin���ݱ�ռ�ձȱ仯����
				{ 
					temp = (unsigned int)(a*sin(2*Pi/N *(i+1))*3600);
				if(temp < deadtime && temp!=0 ){temp = deadtime;}
				else if(temp > (3600 - deadtime)){temp = 3600;}
					
					sintable_OC1[i]     = 3600  -  temp;
					sintable_OC2[N/2+i] = sintable_OC1[i];
				//50hz����(3)
				//sintable_OC1[i]     = 0;
				//sintable_OC2[N/2+i] = sintable_OC1[i];
				
				}
				//
				/*
				//50hz����(4)
				sintable_OC1[0]     = 3600;
				sintable_OC2[N/2] = sintable_OC1[0];
				i--;
				sintable_OC1[i]     = 3600;
				sintable_OC2[N/2+i] = sintable_OC1[i];
				i=N-2;
				*/
				//
				/*
				//������Ϊ��ȷ��sin��������ʱռ�ձ�Ϊ0
				sintable_OC1[i]     = 3600;
				sintable_OC2[N/2+i] = 3600;
				i++;
				*/
				for(;i< N ;i++)//����sin���ݱ�Ϊ0����
				{ 
					sintable_OC1[i]     = 3600 ;
					sintable_OC2[i-N/2] = 3600;
				}
			}
	flag_newdata = 1;
	//}
}


u8 data_H,data_L,temp,data_1,data_2,data_3,data_4,ttt=0;
 //��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{ 
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		times++;
		if(is_con_slave==1)
		{
			//ʧ�ܶ�ʱ�ж�
			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE );
			
			//׼����ӻ���������������Ϣ
			LCD_Print(0,6,"Send-Request----");
			//����������Ϣ����
			printf("AT+CIPSEND=1,3\r\n");
			if(Re(10)==1)
			{
				while(1)//�ȴ������͵�ָ��
				{
					if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
					{if( USART_ReceiveData(USART3) == '>'){break;}}
				}
				//��������������Ϣ
				printf("R\r\n");
				if(Re(10)==1)//���շ��ͳɹ���־
				{
					//LCD_Print(0,6,"Send-Request--OK");
					if(Re(10)==1)//���մӻ���������ʼ��־-OK
					{
						//��ʼ��ʽ��������
						while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != SET){;}
						{data_H = USART_ReceiveData(USART3);}
						while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != SET){;}
						{data_L = USART_ReceiveData(USART3);}
						
						//����\r\n
						while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != SET){;}
						{temp = USART_ReceiveData(USART3);}
						while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != SET){;}
						{temp = USART_ReceiveData(USART3);}
						
						az=(((short)data_H)<<8)  + (short)data_L;
						
						LCD_Print(0,6,"Data-Recevie--OK");
					
					}
					
				}
				else{LCD_Print(0,6,"SendRequest-Fail");}
			}
			else{LCD_Print(0,6,"SendRequest-Fail");}
			
					
			//ʹ�ܶ�ʱ�ж�
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//	TIM1->SR = (uint16_t)~TIM_IT_Update;
}


//��ʱ��1�жϷ������
void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
	static int point=0;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			if(point>=N)
			{
				//����ģʽ
				if(is_impulse==1)
				{
					impulse_i++;
					if(impulse_i==impulse_num)
					{
						TIM_CtrlPWMOutputs(TIM1, DISABLE);
						TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE );
						is_impulse=0;impulse_i=0;
						LCD_Print(35,0,"ֹͣ");
					}
				}
				//
					
				point=0;	
				if(flag_newdata == 1)//����Ҫ����µ�sin��
				{
					 isUseBackup = ~isUseBackup;flag_newdata=0;
				}
			}
			if(isUseBackup==0)
			{
				TIM1->CCR1 = sintable_OC1[point];
				TIM1->CCR2 = sintable_OC1[point];
				TIM1->CCR3 = sintable_OC2[point];
				TIM1->CCR4 = sintable_OC2[point];
			}
			else
			{
				TIM1->CCR1 = sintable_OC1_backup[point];
				TIM1->CCR2 = sintable_OC1_backup[point];
				TIM1->CCR3 = sintable_OC2_backup[point];
				TIM1->CCR4 = sintable_OC2_backup[point];
			}
			point++;	
		}
	   TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//	TIM1->SR = (uint16_t)~TIM_IT_Update;
}



