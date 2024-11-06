//------------头文件-------------------------
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "freertos.h"
#include "task.h"
//-------------------------------------------
uint8_t Number = 0;
//-------------------------------------------
//------------创建任务句柄-------------------
TaskHandle_t LEDTaskHandle;
TaskHandle_t OLEDTaskHandle;
//-------------------------------------------
//-----------任务的回调函数------------------
void LED_TaskFunction(void *pream)
{
	while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		vTaskDelay(500);
	}
}
void OLED_TaskFunction(void *pream)
{
	while(1)
	{
		OLED_ShowNum(1,1,Number,5);
		vTaskDelay(1000);
		Number++;
//		if(Number>100)
//		{
//			vTaskDelete(OLEDTaskHandle);
//		}
	}
}
//-------------------------------------------
//-----------------主函数--------------------
int main(void)
{
	OLED_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//---------------------------------------
	//-------------创建任务(动态调度)--------
	xTaskCreate(LED_TaskFunction,"LED_TASK",512,NULL,1,&LEDTaskHandle);
	xTaskCreate(OLED_TaskFunction,"OLED_TASK",512,NULL,1,&OLEDTaskHandle);
	
	//---------------------------------------
	//--------------开启任务调度-------------
	vTaskStartScheduler();
}
