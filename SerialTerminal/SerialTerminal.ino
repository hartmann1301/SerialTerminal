#include <Adafruit_GFX.h>
#include <Timer.h>
#include <MCUFRIEND_kbv.h>

// the timer object
Timer timer;
MCUFRIEND_kbv tft;

// helping headers
#include "displayHelper.h"
#include "baudRates.h"
#include "coloredLogo.h"

#define TAB_LENGTH 6
const int TAB_RASTER = 6*6;

// 320 * 480 is 60 lines with 8 pixels
int16_t lineHeight = 8;
int16_t topOffset = 8 * lineHeight;
int16_t lines = 52;
int16_t scroll = 0;
int16_t cursorX = 0;
int16_t cursorY = topOffset;

boolean isPageFull = false;
long receivedBytes = 0;

void setup()
{
  // input for BaudRate Button
  pinMode(setBaudPinIn, INPUT_PULLUP);

  // write things to statusBar
  initDisplay();

  tft.setCursor(250, 8);
  tft.print(baudRates[baudCounter], DEC);

  // logo left
  drawLogo(0, 0, terminalLogo, 64, 64);

  // init info timer
  timer.every(1000, updateStatusBar);

  // start receiving data
  Serial.begin(baudRates[baudCounter]);
}


// a function to be executed periodically
void updateStatusBar() {

  writeToDisplay(250, 24, millis() / 1000);
  writeToDisplay(250, 40, receivedBytes);
}

void resetCursor() {
  tft.setCursor(0, topOffset + (lineHeight * lines + scroll * lineHeight) % (lineHeight * lines));
}

void scrollOneLine() {
  // calc color of the new line
  // int16_t tmpColor = scroll % 2 == 0 ? tft.color565(24, 24, 24) : BLACK;

  // clear space for the new line
  tft.fillRect(0, topOffset + scroll * lineHeight, 320, 8, BLACK);

  if (++scroll >= lines)
    scroll = 0;

  tft.vertScroll(topOffset, lineHeight * lines, scroll * lineHeight);
}

void doTabStop() {
  // set new position
  tft.setCursor((tft.getCursorX()/TAB_RASTER + 1)*TAB_RASTER, tft.getCursorY());
}

void checkSerial() {
  // set the cursor to the last postion saved
  tft.setCursor(cursorX, cursorY);

  while (Serial.available()) {

    char data = Serial.read();
    //Serial.print(data);
    receivedBytes++;

    boolean printData = true;

    if (data == '\t') {
      doTabStop();
      printData = false;

      // check if a scroll needs to be done
    } else if (data == '\n' || (tft.getCursorX() > tft.width() - 5)) {
      if (isPageFull) {
        // new line will be done by scrolling!
        if (data == '\n')
          printData = false;

        // scroll because page is full
        resetCursor();
        scrollOneLine();

      } else {
        //Serial.println("empty " + String(tft.getCursorY()));

        // is cursor in the last line?
        if (tft.getCursorY() == tft.height() - lineHeight) {
          isPageFull = true;
        }
      }
    }

    // actual pirnt to screen
    if (printData)
      tft.print(data);
  }

  // save the cursor
  cursorX = tft.getCursorX();
  cursorY = tft.getCursorY();
  //Serial.println("save x: " + String(cursorX) + " y: " + String(cursorY));
}

void loop()
{
  checkSerial();

  checkBaudButton();

  // print infos on screen
  timer.update();
}
