#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"

#define wait_delay HAL_Delay
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif

RTC_HandleTypeDef myRtcHandle;
RTC_AlarmTypeDef myAlarm;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/**
* System Clock Configuration
*/

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	// Enable Power Control clock
	__HAL_RCC_PWR_CLK_ENABLE();
	//The voltage scaling allows optimizing the power
	//consumption when the device is clocked below the
	//maximum system frequency. 
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	//Enable HSE Oscillator and activate PLL
	//with HSE as source
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	//Select PLL as system clock source and configure
	//the HCLK, PCLK1 and PCLK2 clocks dividers
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/*
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    //Configure the main internal regulator output voltage 
    
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    //Initializes the CPU, AHB and APB busses clocks 
    
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


    //Initializes the CPU, AHB and APB busses clocks 
    
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);


  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    //Configure the Systick interrupt time 
    
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    //Configure the Systick 
    
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  //SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
*/



void RTC_Config(void)
{
	//Enable RTC clock
	__HAL_RCC_RTC_ENABLE();
	//RTC Configurations
	myRtcHandle.Instance = RTC;
	myRtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	myRtcHandle.Init.AsynchPrediv = 127;
	myRtcHandle.Init.SynchPrediv = 249;
	myRtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	HAL_RTC_Init(&myRtcHandle);
}

int main(void){
	char buffer[128];
	HAL_Init(); //Init Hardware Abstraction Layer
  SystemClock_Config(); //Config Clocks
	RTC_Config(); //Config Real Time Clock
  GLCD_Initialize(); //Init GLCD
  GLCD_ClearScreen();
  GLCD_SetFont(&GLCD_Font_16x24);
  GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	
	//Set time, data and alarm
	//1) Set time 
	sTime.Hours = 10;
	sTime.Minutes = 10;
	sTime.Seconds = 10;
	HAL_RTC_SetTime(&myRtcHandle, &sTime, RTC_FORMAT_BIN);
	//2) Set date 
	//sDate.Date = 31;
	//sDate.Month = RTC_MONTH_DECEMBER;
	//sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	//sDate.Year = 17;
	//HAL_RTC_SetDate(&myRtcHandle, &sDate, RTC_FORMAT_BIN);
	
	for(;;){
		HAL_RTC_GetTime(&myRtcHandle, &sTime, RTC_FORMAT_BIN);
		sprintf(buffer,"Hours : %i,Min : %i,Sec : %i",sTime.Hours,sTime.Minutes,sTime.Seconds);
		GLCD_DrawString(30,50,buffer);
		wait_delay(500);
	}
	
	
}
