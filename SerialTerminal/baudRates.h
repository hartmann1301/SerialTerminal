boolean buttonIsPushed = false;
boolean buttonWasPushed = false;
byte setBaudPinIn = 19;

byte baudCounter = 2;
byte baudRatesNum = 5;
long baudRates[] = {
  300,
  2400,
  9600,
  57600,
  115200,
};


void changeBaudRate() {

  // increment Baud Counter
  if (baudCounter < baudRatesNum-1) {
    baudCounter++;
  } else {
    baudCounter = 0;
  }

  Serial.end();

  // write new baud rate to display
  writeToDisplay(250, 8, baudRates[baudCounter]);

  Serial.begin(baudRates[baudCounter]);
}

void checkBaudButton() {

  buttonIsPushed = !digitalRead(setBaudPinIn);

  if (buttonIsPushed && !buttonWasPushed)
    changeBaudRate();

  buttonWasPushed = buttonIsPushed;
}

