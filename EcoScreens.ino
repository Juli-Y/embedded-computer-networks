/**
  EcoScreen.ino

  This file contrains the function to draw the Eco screen design and buttons

  Author: Julian Yuste
  Date:   15/01/2023
*/

// ===== Custom Functions ========
//Sets the Display Configurations for Each Screen Selected
void drawEcoScreen()
{
  // Writes the headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(40, 3);
  tft.println("Energy Saving");
  tft.setTextSize(2);
  tft.setCursor(200, 50);
  tft.println("Lighting");
  tft.setCursor(35, 50);
  tft.println("Heating");
  tft.setTextSize(1);
  tft.setCursor(228, 125);
  tft.println("Timer");
  tft.setCursor(63, 125);
  tft.println("Timer");

  // Creates header underlines & screen divider
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);
  tft.drawLine(30, 67, 120, 67, ILI9341_GREEN);
  tft.drawLine(190, 67, 300, 67, ILI9341_GREEN);
  tft.drawLine(225, 135, 260, 135, ILI9341_GREEN);
  tft.drawLine(60, 135, 95, 135, ILI9341_GREEN);
  tft.drawLine(160, 40, 160, 220, ILI9341_GREEN);

  // Draws the buttons
  home_button.initButton(&tft, home_button_x, home_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "HOME", 2, 2);
  home_button.drawButton(true);

  heating_pir_button.initButton(&tft, heating_pir_button_x, heating_pir_button_y, button2_w, button2_h,
                                ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                "PIR ON/OFF", 1, 1);
  heating_pir_button.drawButton(true);

  lighting_pir_button.initButton(&tft, lighting_pir_button_x, lighting_pir_button_y, button2_w, button2_h,
                                 ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                 "PIR ON/OFF", 1, 1);
  lighting_pir_button.drawButton(true);

  lighting_timer_plus_button.initButton(&tft, lighting_timer_plus_x, lighting_timer_plus_y, button2_w, button2_h,
                                        ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                        "+", 1, 1);
  lighting_timer_plus_button.drawButton(true);

  lighting_timer_minus_button.initButton(&tft, lighting_timer_minus_x, lighting_timer_minus_y, button2_w, button2_h,
                                         ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                         "-", 1, 1);
  lighting_timer_minus_button.drawButton(true);

  heating_timer_plus_button.initButton(&tft, heating_timer_plus_x, heating_timer_plus_y, button2_w, button2_h,
                                       ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                       "+", 1, 1);
  heating_timer_plus_button.drawButton(true);

  heating_timer_minus_button.initButton(&tft, heating_timer_minus_x, heating_timer_minus_y, button2_w, button2_h,
                                        ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                        "-", 1, 1);
  heating_timer_minus_button.drawButton(true);
}
