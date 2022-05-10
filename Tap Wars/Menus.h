#include <stdio.h>
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#define wait_delay HAL_Delay
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

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

void WinnerScreen(unsigned int winner,unsigned int count,unsigned int points[]){
	char buffer[128];
	float player1count;
	
	
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
	
	player1count = (float)points[0]*((float)count/1000.0);
	sprintf(buffer,"Player1 tapped %f t/s",player1count);
	GLCD_DrawString(10,30,buffer);
	
	sprintf(buffer,"Player%i Wins",winner);
	GLCD_DrawString(10,10,buffer);
	sprintf(buffer,"%i",count);
	GLCD_DrawString(30,60,buffer);
	
	
}

void FlagHandler(unsigned int flags[],unsigned int points[],unsigned int winner){
	if(flags[0] == 0){
		MainMenu();
	}
	else if(flags[0] == 1){
		GameMenu(points);
	}
	else if(flags[0] == 2){
		WinnerScreen(winner,flags[2],points);
	}
}

