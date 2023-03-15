/**
  HomeScreen.ino

  This file contrains the function to draw the home screen design and buttons

  Author: Julian Yuste
  Date:   21/11/2022
*/

// ===== Custom Functions ========
//Sets the Display Configurations for Each Screen Selected
void drawHomeScreen()
{
  // Writes the headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(120, 3);
  tft.println("HOME");
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(27, 40);
  tft.print("Date      Time     Temp");

  // Creates header underlines
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);
  tft.drawLine(5, 57, 105, 57, ILI9341_GREEN);
  tft.drawLine(130, 57, 225, 57, ILI9341_GREEN);
  tft.drawLine(245, 57, 315, 57, ILI9341_GREEN);

  // Draws the buttons
  heating_button.initButton(&tft, heating_button_x, heating_button_y, button_w, button_h,
                            ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                            "HEATING", 2, 2);
  heating_button.drawButton(true);

  lighting_button.initButton(&tft, lighting_button_x, lighting_button_y, button_w, button_h,
                             ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                             "LIGHTING", 2, 2);
  lighting_button.drawButton(true);

  security_button.initButton(&tft, security_button_x, security_button_y, button_w, button_h,
                             ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                             "SECURITY", 2, 2);
  security_button.drawButton(true);

  eco_button.initButton(&tft, eco_button_x, eco_button_y, button_w, button_h,
                        ILI9341_GREEN, ILI9341_WHITE, ILI9341_GREEN,
                        "ECO", 2, 2);
  eco_button.drawButton(true);
}
