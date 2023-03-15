/**
  SecurityScreen.ino

  This file contrains the function to draw the security screens design
  & contains the functions that carryout the passcode control and reset
  (clear_func)

  Author: Julian Yuste
  Date:   23/02/2023
*/

// ===== Custom Functions ========
//Sets the Display Configurations for Each Screen Selected
void drawSecurityScreen()
{
  // Writes headers to the screen
  // Sets the text colour, background colour, and text size
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 3);
  tft.println("SECURITY");
  tft.setTextSize(2);
  tft.setCursor(90, 50);
  tft.println("Alarm System");

  // Creates header underlines
  tft.drawLine(0, 30, 320, 30, ILI9341_GREEN);
  tft.drawLine(80, 70, 240, 70, ILI9341_GREEN);

  // Draw the buttons
  home_button.initButton(&tft, home_button_x, home_button_y, button3_w, button3_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "HOME", 2, 2);
  home_button.drawButton(true);

  alarm_onoff_button.initButton(&tft, alarm_onoff_button_x, alarm_onoff_button_y, button2_w, button2_h,
                                ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                "ON/OFF", 1, 1);
  alarm_onoff_button.drawButton(true);

  alarm_reset_button.initButton(&tft, alarm_reset_button_x, alarm_reset_button_y, button2_w, button2_h,
                                ILI9341_ORANGE, ILI9341_WHITE, ILI9341_ORANGE,
                                "RESET", 1, 1);
  alarm_reset_button.drawButton(true);
}

void drawPasscodeScreen()
{
  // Draw dashes where each digit will appear once user
  // inputs a number
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 20);
  tft.print("_");
  tft.setCursor(124, 20);
  tft.print("_");
  tft.setCursor(188, 20);
  tft.print("_");
  tft.setCursor(252, 20);
  tft.print("_");

  // Draw the buttons
  home_button.initButton(&tft, home_button_x, home_button_y, button2_w, button2_h,
                         ILI9341_BLUE, ILI9341_WHITE, ILI9341_BLUE,
                         "HOME", 2, 2);
  home_button.drawButton(true);

  zero_button.initButton(&tft, zero_button_x, zero_button_y, button2_w, button2_h,
                         ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                         "0", 2, 2);
  zero_button.drawButton(true);

  one_button.initButton(&tft, one_button_x, one_button_y, button2_w, button2_h,
                        ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                        "1", 2, 2);
  one_button.drawButton(true);

  two_button.initButton(&tft, two_button_x, two_button_y, button2_w, button2_h,
                        ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                        "2", 2, 2);
  two_button.drawButton(true);

  three_button.initButton(&tft, three_button_x, three_button_y, button2_w, button2_h,
                          ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                          "3", 2, 2);
  three_button.drawButton(true);

  four_button.initButton(&tft, four_button_x, four_button_y, button2_w, button2_h,
                         ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                         "4", 2, 2);
  four_button.drawButton(true);

  five_button.initButton(&tft, five_button_x, five_button_y, button2_w, button2_h,
                         ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                         "5", 2, 2);
  five_button.drawButton(true);

  six_button.initButton(&tft, six_button_x, six_button_y, button2_w, button2_h,
                        ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                        "6", 2, 2);
  six_button.drawButton(true);

  seven_button.initButton(&tft, seven_button_x, seven_button_y, button2_w, button2_h,
                          ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                          "7", 2, 2);
  seven_button.drawButton(true);

  eight_button.initButton(&tft, eight_button_x, eight_button_y, button2_w, button2_h,
                          ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                          "8", 2, 2);
  eight_button.drawButton(true);

  nine_button.initButton(&tft, nine_button_x, nine_button_y, button2_w, button2_h,
                         ILI9341_PURPLE, ILI9341_WHITE, ILI9341_PURPLE,
                         "9", 2, 2);
  nine_button.drawButton(true);
}

void passcode_func()
{
  // Every time this function is called, it will print
  // the digit on the tft display, and save the following
  // digit in the next slot of the array.
  // The passcode will also be printed to the serial monitor.
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(digit[slot], 20);
  tft.print(input_num);
  input_passcode[slot] = input_num;
  delay(250);
  slot = slot + 1;
  Serial.print("Input Passcode =");
  Serial.print(input_passcode[0]);
  Serial.print(input_passcode[1]);
  Serial.print(input_passcode[2]);
  Serial.println(input_passcode[3]);
}

void clear_func()
{
  // When the clear function is called, the input passcode
  // array will be reset back to 0 & dashes will be printed
  // on the tft display, ready for the next attempt.
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(2);
  memset(input_passcode, 0, sizeof(input_passcode));
  slot = 0;
  tft.setCursor(60, 20);
  tft.print("_");
  tft.setCursor(124, 20);
  tft.print("_");
  tft.setCursor(188, 20);
  tft.print("_");
  tft.setCursor(252, 20);
  tft.print("_");
}
