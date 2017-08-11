#include <Adafruit_GFX.h>
#include <Timer.h>
#include <MCUFRIEND_kbv.h>

// the timer object
Timer timer;
MCUFRIEND_kbv tft;

#include "globals.h"

// helping headers
#include "displayHelper.h"
#include "buttonHelper.h"

#include "coloredLogo.h"

void setup()
{
  // input for BaudRate Button
  pinMode(buttonPin, INPUT_PULLUP);

  // write things to statusBar
  initDisplay();

  // logo left
  drawLogo(0, 0, terminalLogo, 64, 64);

  // init statusbar timer
  timer.every(1000, updateStatusBar);

  // start receiving data
  Serial.begin(baudRates[baudRateCurrent]);
  Serial.println("Start SerialTerminal:");

  /*
  // ist takes about 2ms to print one Letter, that is very low
  long measureStart = micros();
  tft.print(".");
  // get measure duration
  long measureDuration = micros() - measureStart;
  Serial.println(measureDuration, DEC);
  */
}

void resetCursor() {
  /*
  // save the cursor position for deleting
  cursorPositionsList[cursorPositionsCounter] = tft.getCursorX();

  if (cursorPositionsCounter < lines) {
    cursorPositionsCounter++;
  } else {
    cursorPositionsCounter = 0;
  }

  */
  tft.setCursor(0, topOffset + (lineHeight * lines + scroll * lineHeight) % (lineHeight * lines));
}

void scrollOneLine() {
  // calc color of the new line
  // int16_t tmpColor = scroll % 2 == 0 ? tft.color565(24, 24, 24) : BLACK;

  // clear space for the new line
  tft.fillRect(0, topOffset + scroll * lineHeight, 320, 7, BLACK);

  if (++scroll >= lines)
    scroll = 0;

  tft.vertScroll(topOffset, lineHeight * lines, scroll * lineHeight);
}

void doTabStop() {
  // set new position
  tft.setCursor((tft.getCursorX() / TAB_RASTER + 1)*TAB_RASTER, tft.getCursorY());
}

void checkSerial() {
  // set the cursor to the last postion saved
  tft.setCursor(cursorX, cursorY);

  while (Serial.available()) {

    char data = Serial.read();
    //Serial.print(data);
    receivedBytes++;
    currUpstream++;

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

  checkButton();

  // print infos on screen
  timer.update();
}
