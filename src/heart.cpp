#include <heart.h>
#include <EEPROM.h>
#ifdef __USE_IR
  #include <ir.h>
#endif
#ifdef __USE_BUTTON
  #include <button.h>
#endif

uint8_t XY( uint8_t x, uint8_t y)
// returns ERROR_LED for non existing LED
{
  if (x >= WIDTH) x = WIDTH-1;
  if (y >= HEIGHT) y = HEIGHT-1;

  return xy[y][x] - 1;
  
}

boolean is_serv = false;
uint32_t Next_Show;
uint32_t Prev_Show;
uint32_t Bright_Down;
uint32_t Bright_Up;
uint32_t Randomize_Pattern;
uint32_t Color_White;
uint32_t Color_Red;
uint32_t Color_Blue;
uint32_t Blending_Overlay;
uint32_t Off;
uint32_t Speed_Dec;
uint32_t Speed_Inc;
uint32_t Runtime_Dec;
uint32_t Runtime_Inc;
uint32_t WiFi_On;

int menu_index = 0;    //  allows to cycle through the menu
                              //  0 - select show (Next_Show, Prev_Show)
                              //  1 - brightness
                              //  2 - speed
                              //  3 - runtime
                              //  4 - random show toggle

boolean is_server() {
  return is_serv;
}

void setup_heart() {

    EEPROM.begin(EEPROM_SIZE);

    // firgure out the role / sender or just receiver?
    is_serv = EEPROM.read(0);
    if (is_serv)
      Serial.println("The Heart - Server Mode");
    else
      Serial.println("The Heart - Client Mode");

    // TODO - we'll see if the buttons have been set up already
    // if both buttons 1 and 2 are pressed at startup, we put this guy into server mode
    if ( digitalRead(BUTTON_1_PIN) && digitalRead(BUTTON_2_PIN) ) {
      Serial.println("Startup - setting to server");
      is_serv = true;
      EEPROM.write(0, is_serv);
      EEPROM.commit();
    }
    // if both buttons 3 and 4 are pressed at startup, we put this guy into server mode
    if ( digitalRead(BUTTON_3_PIN) && digitalRead(BUTTON_4_PIN) ) {
      Serial.println("Startup - setting to client");
      is_serv = false;
      EEPROM.write(0, is_serv);
      EEPROM.commit();
    }

    #ifdef __USE_IR
    if (is_serv) {
      // setup the wide IR
      Next_Show = Button_Green;
      Prev_Show = Button_Red;
      Bright_Down = Button_BrightDown;
      Bright_Up = Button_BrightUp; 
      Randomize_Pattern = Button_Flash;
      Color_White = Button_White;
      Color_Red = Button_1;         // NULL
      Color_Blue = Button_Blue;
      Blending_Overlay = Button_Strobe;
      Off = Button_Off;
      Speed_Dec = Button_Orange;
      Speed_Inc = Button_LightGreen;
      Runtime_Dec = Button_LightRed;
      Runtime_Inc = Button_LightLightBlue;
      WiFi_On = Button_On;

    } else {
      // setup the narrow IR
      Next_Show = Button_Forw;
      Prev_Show = Button_Rew;
      Bright_Down = Button_Silent;
      Bright_Up = Button_Loud;
      Randomize_Pattern = Button_Play;
      Color_White = Button_0;
      Color_Red = Button_7;
      Color_Blue = Button_8;
      Blending_Overlay = Button_EQ;
      Off = Button_Return;
      Speed_Dec = Button_CH_Minus;
      Speed_Inc = Button_CH_Plus;
      Runtime_Dec = Button_1;
      Runtime_Inc = Button_2;
      WiFi_On = Button_3;

   }
   #endif
   #ifdef __USE_BUTTON
      Next_Show = Button_Green;
      Prev_Show = Button_Red;
      Bright_Down = Button_BrightDown;
      Bright_Up = Button_BrightUp; 
      Randomize_Pattern = Button_Flash;
      Color_White = Button_White;
      Color_Blue = Button_Blue;
      Color_Red = Button_Red;
      Blending_Overlay = Button_Strobe;
      Off = Button_Off;
      Speed_Dec = Button_Orange;
      Speed_Inc = Button_LightGreen;
      Runtime_Dec = Button_LightRed;
      Runtime_Inc = Button_LightLightBlue;
      WiFi_On = Button_On;

   #endif

}