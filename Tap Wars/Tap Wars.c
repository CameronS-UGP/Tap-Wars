#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "Menus.h"
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

unsigned int* Initpoints(unsigned int point, unsigned int points[]){
	int i;
	for(i = 0; i<2; i++){
		points[i] = point;
	}
	return points;
}


int CheckWin(unsigned int points[],unsigned int goal){
	if(points[0] >= goal){
		return 1;
	}
	else if(points[1] >= goal){
		return 2;
	}
	return 0;
}

unsigned int ButtonHandler(unsigned int flags[],unsigned int points[]){
	unsigned int winner;
	TOUCH_STATE tsc_state;
	Touch_GetState(&tsc_state);
	winner = CheckWin(points,flags[1]);
	//On the main menu
	if(flags[0] == 0){
		if((tsc_state.x >= 20 && tsc_state.x <= 100) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//100 points
			flags[0] = 1;
			flags[1] = 100;
			FlagHandler(flags,points,winner);
		}
		else if((tsc_state.x >= 140 && tsc_state.x <= 220) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//150 points
			flags[0] = 1;
			flags[1] = 150;
			FlagHandler(flags,points,winner);
		}
		else if((tsc_state.x >= 260 && tsc_state.x <= 340) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//200 points
			flags[0] = 1;
			flags[1] = 200;
			FlagHandler(flags,points,winner);
		}
		else if((tsc_state.x >= 380 && tsc_state.x <= 480) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
			//250 points
			flags[0] = 1;
			flags[1] = 250;
			FlagHandler(flags,points,winner);
		}
	}
	//On the game menu
	else if(flags[0] == 1){
		if(winner > 0){
			flags[0] = 2;
			FlagHandler(flags,points,winner);
		}
		if(tsc_state.pressed){ //wil be replaced with if button pressed
			if(tsc_state.x < 240 && tsc_state.x > 0){
				points[0]++;	//Player1
				FlagHandler(flags,points,winner);
			}
			else if(tsc_state.x > 240 && tsc_state.x < 480){
				points[1]++;	//Player2
				FlagHandler(flags,points,winner);
			}
		}
	}
	wait_delay(100);
	flags[2]++;
	return *flags;
}





int main(void){
	unsigned int flags[3];
	unsigned int points[2];
	
	int i;
	HAL_Init(); //Init Hardware Abstraction Layer
  SystemClock_Config(); //Config Clocks
	Touch_Initialize(); //Init Touchscreen
  GLCD_Initialize(); //Init GLCD
	GLCD_ClearScreen();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	flags[0] = 0; //Screen flag
	//flags[1] Goal flag
	flags[2] = 0; //system loop count
	
	for(i = 0; i<2; i++){
		points[i] = 0;
	}
	//GameMenu();
	//MainMenu();
	FlagHandler(flags,points,0);
	for(;;){
		*flags = ButtonHandler(flags,points);
		
	}
}
