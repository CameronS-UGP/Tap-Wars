/**
  ******************************************************************************
  * @file    TapWars.c
  * @author  Andrew Sturdy, Cameron Shipman
  * @version V1.0.0
  * @date    13-May-2022
  * @brief   This file provides all program functions
  ******************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
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

/** @defgroup MAIN
* @brief main file
* @{
*/ 

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


/** @defgroup Screens
* @{
*/ 

/**
  * @brief This function turns on and of GPIO pins needed to control the buzzer and LED.
  * @note This function is hard coded to turn on D3 & D4.
  * @param None
  * @retval None
  */
void Periphral(void){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);
	wait_delay(10);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
}

/**
  * @brief This function draws the Win screen and controls the button on the screen.
	* @param winner : Who won, points[] : the array holding each players points, tsc_state : the state of the screen.
  * @retval NA (Returns 0 to exit function).
  */
int Winner(int winner,int points[],TOUCH_STATE tsc_state){
	char buffer[128];
	
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor(GLCD_COLOR_GREEN);

	sprintf(buffer,"Player%i Wins",winner);
	GLCD_DrawString(10,10,buffer);
	
	if(winner == 1){
		sprintf(buffer,"Player2s score was : %i",points[1]);
	}
	else if(winner == 2){
		sprintf(buffer,"Player1s score was : %i",points[0]);
	}
	
	GLCD_DrawString(10,50,buffer);
	
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	GLCD_DrawRectangle(20,173,80,40);
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(35,190,"Main Menu");
	for(;;){
		Touch_GetState(&tsc_state);
		if((tsc_state.x >= 20 && tsc_state.x <= 100) && (tsc_state.y >= 173 && tsc_state.y <= 213)){
			return 0;
		}
		Periphral();
	}
}

/**
  * @brief This function draws the game screen and controls the gaining of points.
  * @note This function only works with external GPIO pins D1 & D2.
	* @param goal : The amount of points required to win, tsc_state : the state of the screen.
  * @retval NA (Returns 0 to exit function).
  */


int Game(int goal,TOUCH_STATE tsc_state){
	int points[2];
	char buffer[128];
	GPIO_PinState player1;
	GPIO_PinState player2;
	int button1;
	int button2;
	int loop = 1;
	GLCD_ClearScreen();
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_DrawVLine(240,0,270);
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_SetFont(&GLCD_Font_16x24);
	points[0] = 0;
	points[1] = 0;
	
	sprintf(buffer,"Goal : %i",goal);
	GLCD_DrawString(20,243,buffer);
	while(loop != 0){
		sprintf(buffer,"Points: %i",points[0]);
		GLCD_DrawString(20,10,buffer); //Player1s points
	
		sprintf(buffer,"Points: %i",points[1]);
		GLCD_DrawString(260,10,buffer); //Player2s points
	
		//Touch_GetState(&tsc_state);
		
		player1 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6); //Player1 D1
		player2 = HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //Player2 D2
		
		
		
		//check if the current state of button1 is pressed and the previous state is not pressed
		if(button1 == 0 && !player1){
			points[0] = points[0]+2;//Player1
			points[1]--; //Player2
			button1 = 1;
		}
		//prev state pressed current not pressed reset
		else if(button1 == 1 && player1){
			button1 = 0;
		}
		
		//check if the current state of button1 is pressed and the previous state is not pressed
		if(button2 == 0 && !player2){
			points[1] = points[1]+2;//Player2
			points[0]--; //Player1
			button2 = 1;
		}
		//prev state pressed current not pressed reset
		else if(button2 == 1 && player2){
			button2 = 0;
		}
		
		if(points[0] >= goal){
			loop = Winner(1,points,tsc_state);
			return 0;
		}
		else if(points[1] >= goal){
			loop = Winner(2,points,tsc_state);
			return 0;
		}
	}
	return 0;
}


/**
  * @brief This function draws the main menu and controls all the buttons on the main menu.
	* @param None
  * @retval NA (Returns 0 to exit function).
  */

int MainMenu(void){
	TOUCH_STATE tsc_state;
	char buffer[128];
	int goal;
	int loop = 1;
	HAL_Init(); //Init Hardware Abstraction Layer
  SystemClock_Config(); //Config Clocks
	Touch_Initialize(); //Init Touchscreen
  GLCD_Initialize(); //Init GLCD
	
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor(GLCD_COLOR_MAGENTA);
	GLCD_DrawString(10,10,"TAP WARS");
	
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	GLCD_DrawString(10,40,"The aim is to reach the required amount of points before your opponent.");
	GLCD_DrawString(10,60,"To get points press the button. ");
	GLCD_DrawString(10,80,"The button gives you 2 points and makes your opponent lose 1.");
	GLCD_DrawString(10,100,"To start choose a goal using the buttons bellow.");
	
	GLCD_DrawRectangle(20,203,80,40); // Button1 x=20 to 100, y=203 to 243
	GLCD_DrawRectangle(140,203,80,40); // Button2 x=140 to 220, y=203 to 243
	GLCD_DrawRectangle(260,203,80,40); // Button3 x=260 to 340, y=203 to 243
	GLCD_DrawRectangle(380,203,80,40); // Button4 x=380 to 480, y=203 to 243
	
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	sprintf(buffer,"%i",100); //Button1
	GLCD_DrawString(35,220,buffer);
	sprintf(buffer,"%i",150); //Button2
	GLCD_DrawString(155,220,buffer);
	sprintf(buffer,"%i",200); //Button3
	GLCD_DrawString(275,220,buffer);
	sprintf(buffer,"%i",250); //Button4
	GLCD_DrawString(395,220,buffer);
	while(loop != 0){
		Touch_GetState(&tsc_state);
		if(tsc_state.pressed){
			if((tsc_state.x >= 20 && tsc_state.x <= 100) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
				//100 points
				goal = 100;
				loop = Game(goal,tsc_state);
				wait_delay(100);
			}
			else if((tsc_state.x >= 140 && tsc_state.x <= 220) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
				//150 points
				goal = 150;
				loop = Game(goal,tsc_state);
				wait_delay(100);
			}
			else if((tsc_state.x >= 260 && tsc_state.x <= 340) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
				//200 points
				goal = 200;
				loop = Game(goal,tsc_state);
				wait_delay(100);
			}	
			else if((tsc_state.x >= 380 && tsc_state.x <= 480) && (tsc_state.y >= 203 && tsc_state.y <= 243)){
				//250 points
				goal = 250;
				loop = Game(goal,tsc_state);
				wait_delay(100);
			}
		}
	}
	return 0;
}

/**
* @}
*/ 

/** @defgroup Main_Loop
* @{
*/ 

/**
  * @brief (Main Loop) This function initialises GPIO pins and calls the mainmenu screen.
	* @note This function initialises D0,D1,D2,D3 & D4
	* @param None
  * @retval None
  */
int main(void){
	
	GPIO_InitTypeDef gpio0;
	GPIO_InitTypeDef gpio1;
	GPIO_InitTypeDef gpio2;
	GPIO_InitTypeDef gpio3;
	GPIO_InitTypeDef gpio4;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	
	gpio0.Mode = GPIO_MODE_OUTPUT_PP;
	gpio0.Pull = GPIO_NOPULL;
	gpio0.Speed = GPIO_SPEED_HIGH;
	gpio0.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &gpio0);
	
	gpio1.Mode = GPIO_MODE_INPUT;
	gpio1.Pull = GPIO_PULLUP;
	gpio1.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOC, &gpio1);
	
	gpio2.Mode = GPIO_MODE_INPUT;
	gpio2.Pull = GPIO_PULLUP;
	gpio2.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOG, &gpio2);
	
	gpio3.Mode = GPIO_MODE_OUTPUT_PP;
	gpio3.Pull = GPIO_NOPULL;
	gpio3.Speed = GPIO_SPEED_HIGH;
	gpio3.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &gpio3);
	
	gpio4.Mode = GPIO_MODE_OUTPUT_PP;
	gpio4.Pull = GPIO_NOPULL;
	gpio4.Speed = GPIO_SPEED_HIGH;
	gpio4.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOG, &gpio4);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
	
	while(1){
		MainMenu();
	}
}
/**
* @}
*/ 
/**
* @}
*/

