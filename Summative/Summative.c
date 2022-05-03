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


/**
* System Clock Configuration
*/
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSE Oscillator and activate PLL
	with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Select PLL as system clock source and configure
	the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void GameMenu(unsigned int points[]){
	char buffer[128];
	GLCD_ClearScreen();
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_DrawVLine(240,0,270);
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_SetFont(&GLCD_Font_16x24);
	
	sprintf(buffer,"Points: %i",points[0]);
	GLCD_DrawString(20,10,buffer); //Player1s points
	
	sprintf(buffer,"Points: %i",points[1]);
	GLCD_DrawString(260,10,buffer); //Player2s points
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	
}

void MainMenu(){
	char buffer[128];
	GLCD_ClearScreen();
	
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor(GLCD_COLOR_MAGENTA);
	GLCD_DrawString(10,10,"INSERT NAME HERE");
	
	
  GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	
	//GLCD_DrawRectangle(20,20,80,40); //Button1 x=50 to 130, y=20 to 60
	//GLCD_DrawString(37,35,"MainMenu");
	
	
	//This can change depending on what you want (These are just place holders)
	//--------------------------------------
	
	
	GLCD_DrawRectangle(20,203,80,40); // Button2 x=20 to 100, y=203 to 243
	
	GLCD_DrawRectangle(140,203,80,40); // Button3 x=140 to 220, y=203 to 243
	
	GLCD_DrawRectangle(260,203,80,40); // Button4 x=260 to 340, y=203 to 243
	
	GLCD_DrawRectangle(380,203,80,40); // Button5 x=380 to 480, y=203 to 243
	
	//--------------------------------------
	
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	
	sprintf(buffer,"%i",100); //Button2
	GLCD_DrawString(35,220,buffer);
	
	sprintf(buffer,"%i",150); //Button3
	GLCD_DrawString(155,220,buffer);
	
	sprintf(buffer,"%i",200); //Button4
	GLCD_DrawString(275,220,buffer);
	
	sprintf(buffer,"%i",250); //Button5
	GLCD_DrawString(395,220,buffer);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
}

void FlagHandler(int flag,unsigned int points[]){
	if(flag == 0){
		MainMenu();
	}
	else if(flag == 1){
		GameMenu(points);
	}
}



int ButtonHandler(unsigned int flag,unsigned int points[]){
	TOUCH_STATE tsc_state;
	Touch_GetState(&tsc_state);
	//On the main menu
	if(flag == 0){
		if((tsc_state.x >= 20 && tsc_state.x <= 100) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//100 points
			flag = 1;
			FlagHandler(flag,points);
		}
		else if((tsc_state.x >= 140 && tsc_state.x <= 220) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//150 points
			flag = 1;
			FlagHandler(flag,points);
		}
		else if((tsc_state.x >= 260 && tsc_state.x <= 340) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//200 points
			flag = 1;
			FlagHandler(flag,points);
		}
		else if((tsc_state.x >= 380 && tsc_state.x <= 480) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//250 points
			flag = 1;
			FlagHandler(flag,points);
		}
	}
	return flag;
}





int main(void){
	unsigned int flag;
	unsigned int points[2];
	int i;
	HAL_Init(); //Init Hardware Abstraction Layer
  SystemClock_Config(); //Config Clocks
	Touch_Initialize(); //Init Touchscreen
  GLCD_Initialize(); //Init GLCD
	GLCD_ClearScreen();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	flag = 0;

	for(i = 0; i<2; i++){
		points[i] = 250;
	}
	//GameMenu();
	//MainMenu();
	FlagHandler(flag,points);
	for(;;){
		flag = ButtonHandler(flag,points);
	}
}
