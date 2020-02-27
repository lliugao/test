#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#define SMOG_PIN46_R	1000


int main(void)
{ 
	u16 adcx1;
	u16 adcx;
	u16 Rs,n=0;
	u16 temp_val;
	u16 normal=9;
	float temp;
	u8 temperature;  	    
	u8 humidity;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);    //³õÊ¼»¯ÑÓÊ±º¯Êý
	uart_init(115200);	//³õÊ¼»¯´®¿Ú²¨ÌØÂÊÎª115200
	printf("AT+CIPMUX=1\r\n");  
	delay_ms(300);
	printf("AT+CIPSERVER=1,8086\r\n"); 
	delay_ms(300);
	Adc_Init1(); 
	DHT11_Init();	
	LED_Init();
	while(1)
	{ 
		adcx1=Get_Adc_Average1(ADC_Channel_6,20);//»ñÈ¡Í¨µÀ5µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾  
		adcx1=adcx1/4;
		DHT11_Read_Data(&temperature,&humidity);	
		adcx=Get_Adc_Average(ADC_Channel_5,20);//»ñÈ¡Í¨µÀ5µÄ×ª»»Öµ£¬20´ÎÈ¡Æ½¾ù

		temp=(float)adcx*(3.3/4096);          //»ñÈ¡¼ÆËãºóµÄ´øÐ¡ÊýµÄÊµ¼ÊµçÑ¹Öµ£¬±ÈÈç3.1111
		temp_val = adcx;
		if(temp<0.9)
			{
				n=temp/0.001;
				Rs = n*0.012;
				printf("AT+CIPSEND=0,50\r\n"); 
				delay_ms(300);
				printf("%dK%dK%dK%dK",humidity,temperature,Rs,adcx1);
			}
		else
			{
				Rs = ((temp-0.9)/0.1)*100;
				Rs =Rs+normal;
				printf("AT+CIPSEND=0,60\r\n"); 
				delay_ms(300);
				printf("%dK%dK%dK%dK",humidity,temperature,Rs,adcx1);
			}
				delay_ms(1000);
		if(Rs>1000||adcx1>600)
			{
				GPIO_ResetBits(GPIOF,GPIO_Pin_9); // DS0À­µÍ£¬ÁÁ   µÈÍ¬LED0=0;
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
				delay_ms(300);
				GPIO_SetBits(GPIOF,GPIO_Pin_9); // DS0À­¸ß   µÈÍ¬LED0=1;
				GPIO_SetBits(GPIOF,GPIO_Pin_10);
				delay_ms(300);				
			}		
		delay_ms(300);			
	}
}



