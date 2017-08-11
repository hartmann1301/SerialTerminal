boolean buttonIsPushed = false;
boolean buttonWasPushed = false;

unsigned long buttonPressedTime = 0;

const byte buttonPin = 19;
const int pressShortTime = 500;
const int pressLongTime = 1500;

void changeBaudRate() {

  // increment Baud Counter
  if (baudRateCurrent < baudRatesNum - 1) {
    baudRateCurrent++;
  } else {
    baudRateCurrent = 0;
  }

  Serial.end();

  // write new baud rate to display
  updateVar(baudRates[baudRateCurrent], xPosVariables, yPosBaud, 6);

  Serial.begin(baudRates[baudRateCurrent]);
}

void changeTextColor() {

  // increment TextColor Counter
  if (textColorCurrent < textColorNum - 1) {
    textColorCurrent++;
  } else {
    textColorCurrent = 0;
  }

  // actuall set the color
  tft.setTextColor(textColors[textColorCurrent]);

  // manipulate lastValus variables to force a repaint
  lastMinutes = -1;
  lastHours = -1;
  lastReceivedBytes = -1;
  lastUpstream = -1;
  forcedSecondsRepaint = true;
  forcedTotalUnitRepaint = true;
}

void checkButton() {

  buttonIsPushed = !digitalRead(buttonPin);

  // dectect possitve Edge
  if (buttonIsPushed && !buttonWasPushed) {
    buttonPressedTime = millis();

    // print a small pushed hint
    tft.drawBitmap(xPosDescription - 8, yPosBaud, cursurIcon, 8, 8, textColors[textColorCurrent]);
  }

  // detect negative Edge
  if (!buttonIsPushed && buttonWasPushed) {
    int tmpPressedTime = millis() - buttonPressedTime;

    // erase a small pushed hint
    tft.drawBitmap(xPosDescription - 8, yPosBaud, cursurIcon, 8, 8, BLACK);

    // decide what to do
    if (tmpPressedTime < pressShortTime)
      changeBaudRate();
  }

  // check for long press
  if (buttonIsPushed) {
    int tmpPressedTime = millis() - buttonPressedTime;

    if (tmpPressedTime > pressLongTime) {
      // add one diff off the presses, so it will not detect a short press
      buttonPressedTime += (pressLongTime - pressShortTime);

      // actual color changing
      changeTextColor();

      // repaint the cursor 
      tft.drawBitmap(xPosDescription - 8, yPosBaud, cursurIcon, 8, 8, textColors[textColorCurrent]);
      updateVar(baudRates[baudRateCurrent], xPosVariables, yPosBaud, 0);
    }
  }

  // what was once new is now old
  buttonWasPushed = buttonIsPushed;
}

