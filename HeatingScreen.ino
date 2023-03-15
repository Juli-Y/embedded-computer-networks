/**
  HeatingScreen.ino

  This file contrains the function to draw the heating screen design and buttons

  Author: Julian Yuste
  Date:   15/01/2023
*/

// ===== Custom Functions ========
//Sets the Display Configurations for Each Screen Selected
void drawHeatingScreen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("HEATING");
  tft.setTextSize(2);
  tft.setCursor(10, 130);
  tft.println("Room Temp");
  tft.setCursor(150, 50);
  tft.println("Heater System");
  tft.setCursor(10, 50);
  tft.println("Humidity");
  tft.setCursor(175, 130);
  tft.println("Set Temp");

  // Creates header underlines and screen divider
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);
  tft.drawLine(3, 67, 110, 67, ILI9341_GREEN);
  tft.drawLine(145, 67, 310, 67, ILI9341_GREEN);
  tft.drawLine(6, 147, 117, 147, ILI9341_GREEN);
  tft.drawLine(172, 147, 270, 147, ILI9341_GREEN);
  tft.drawLine(130, 40, 130, 220, ILI9341_GREEN);

  // Draws the buttons
  home_button.initButton(&tft, home_button_x, home_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "HOME", 2, 2);
  home_button.drawButton(true);

  heater_onoff_button.initButton(&tft, heater_onoff_button_x, heater_onoff_button_y, button2_w, button2_h,
                                 ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                 "ON/OFF", 1, 1);
  heater_onoff_button.drawButton(true);

  plus_button.initButton(&tft, plus_button_x, plus_button_y, button2_w, button2_h,
                         ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                         "+", 2, 2);
  plus_button.drawButton(true);

  minus_button.initButton(&tft, minus_button_x, minus_button_y, button2_w, button2_h,
                          ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                          "-", 2, 2);
  minus_button.drawButton(true);
}
