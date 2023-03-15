/**
  LightingScreen.ino

  This file contrains the function to draw the lighting screens design and buttons
  The screens are: Main lighting screen, outdoor screen, zone1, 2 & 3 screens

  Author: Julian Yuste
  Date:   21/11/2022
*/

// ===== Custom Functions ========
//Sets the Display Configurations for Each Screen Selected
void drawLightingScreen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("LIGHTING");
  tft.setTextSize(2);
  tft.setCursor(5, 50);
  tft.println("Lighting Status");

  // Creates header underlines and screen divider
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);
  tft.drawLine(0, 68, 185, 68, ILI9341_GREEN);

  // Draws the buttons
  home_button.initButton(&tft, home_button_x, home_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "HOME", 2, 2);
  home_button.drawButton(true);

  outdoor_button.initButton(&tft, outdoor_button_x, outdoor_button_y, button_w, button_h,
                            ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                            "OUTDOOR", 2, 2);
  outdoor_button.drawButton(true);

  zone1_button.initButton(&tft, zone1_button_x, zone1_button_y, button_w, button_h,
                          ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                          "ZONE 1", 2, 2);
  zone1_button.drawButton(true);

  zone2_button.initButton(&tft, zone2_button_x, zone2_button_y, button_w, button_h,
                          ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                          "ZONE 2", 2, 2);
  zone2_button.drawButton(true);

  zone3_button.initButton(&tft, zone3_button_x, zone3_button_y, button_w, button_h,
                          ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                          "ZONE 3", 2, 2);
  zone3_button.drawButton(true);
}

void drawOutdoorScreen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("OUTDOOR");
  tft.setTextSize(1);
  tft.setCursor(10, 60);
  tft.println("Always OFF");
  tft.setCursor(255, 60);
  tft.println("Always ON");
  tft.setCursor(100, 135);
  tft.println("Light Sensor Level");

  // Creates header underlines
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);

  // Sets the coordinates and draw a green line. (light sensor scale)
  tft.drawLine(70, 75, 70, 130, ILI9341_GREEN);
  tft.drawLine(240, 75, 240, 130, ILI9341_GREEN);
  tft.drawLine(155, 84, 155, 120, ILI9341_GREEN);
  tft.drawLine(198, 84, 198, 120, ILI9341_GREEN);
  tft.drawLine(113, 84, 113, 120, ILI9341_GREEN);

  // Draws the buttons
  back_button.initButton(&tft, back_button_x, back_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "BACK", 2, 2);
  back_button.drawButton(true);

  plus_button.initButton(&tft, plus_button_x, plus_button_y, button2_w, button2_h,
                         ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                         "+", 2, 2);
  plus_button.drawButton(true);

  minus_button.initButton(&tft, minus_button_x, minus_button_y, button2_w, button2_h,
                          ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                          "-", 2, 2);
  minus_button.drawButton(true);
}

void drawZone1Screen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("ZONE 1");
  tft.setCursor(65, 85);
  tft.println("Lights");
  tft.setTextSize(1);
  tft.setCursor(248, 65);
  tft.println("MAX");
  tft.setCursor(248, 190);
  tft.println("MIN");

  // Creates header underlines
  tft.drawLine(60, 113, 175, 113, ILI9341_GREEN);
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);

  // Draw the buttons
  back_button.initButton(&tft, back_button_x, back_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "BACK", 2, 2);
  back_button.drawButton(true);
}

void drawZone2Screen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("ZONE 2");
  tft.setCursor(65, 85);
  tft.println("Lights");
  tft.setTextSize(1);
  tft.setCursor(248, 65);
  tft.println("MAX");
  tft.setCursor(248, 190);
  tft.println("MIN");

  // Creates header underlines
  tft.drawLine(60, 113, 175, 113, ILI9341_GREEN);
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);

  // Draw the buttons
  back_button.initButton(&tft, back_button_x, back_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "BACK", 2, 2);
  back_button.drawButton(true);
}

void drawZone3Screen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("ZONE 3");
  tft.setCursor(65, 85);
  tft.println("Lights");
  tft.setTextSize(1);
  tft.setCursor(248, 65);
  tft.println("MAX");
  tft.setCursor(248, 190);
  tft.println("MIN");

  // Creates header underlines
  tft.drawLine(60, 113, 175, 113, ILI9341_GREEN);
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);

  // Draw the buttons
  back_button.initButton(&tft, back_button_x, back_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "BACK", 2, 2);
  back_button.drawButton(true);
}
