const int xPosTitle = 70;
const int xPosDescription = 180;
const int xPosVariables = 250;
const int xPosUnits = 295;

const int yPosOnline = 4;
const int yPosUpstream = 16;
const int yPosTotal = 28;
const int yPosBaud = 52;

char volatileTimeValue[3] = "00";
String volatileUpstreamUnit = "B/s";
String volatileTotalUnit = "Byte";

static const unsigned char PROGMEM cursurIcon[] =
{
  B00000000,
  B10000000,
  B11000000,
  B11100000,

  B11110000,
  B11100000,
  B11000000,
  B10000000
};

// clear space for the new line
void clearSpace(const int &x, const int &y, const byte &len) {
  tft.fillRect(x, y, len * 5 + len, 8, BLACK);
}

void updateText(const String &text, const int &x, const int &y, const byte &len) {
  clearSpace(x, y, len);

  tft.setCursor(x, y);
  tft.print(text);
}

void updateVar(const long &var, const int &x, const int &y, const byte &len) {
  clearSpace(x, y, len);

  tft.setCursor(x, y);
  tft.print(var, DEC);
}

void initDisplay() {
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(0);     //Portrait
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);

  tft.setTextSize(2);
  tft.setCursor(xPosTitle, 8);
  tft.print("Serial");

  tft.setCursor(xPosTitle, 32);
  tft.print("Terminal");
  tft.setTextSize(1);

  updateText("Baudrate", xPosDescription, yPosBaud, 0);
  updateVar(baudRates[baudRateCurrent], xPosVariables, yPosBaud, 0);
  updateText("B/s", xPosUnits, yPosBaud, 0);

  updateText("Online for", xPosDescription, yPosOnline, 0);
  updateText("00:00:00", xPosVariables, yPosOnline, 0);

  updateText("Upstream", xPosDescription, yPosUpstream, 0);
  updateText(volatileUpstreamUnit, xPosUnits, yPosUpstream, 0);

  updateText("Total", xPosDescription, yPosTotal, 0);
}

void printOnlineTime() {
  // calc time string
  long tmpTotalSeconds = millis() / 1000;
  byte tmpSeconds = tmpTotalSeconds % 60;
  byte tmpMinutes = tmpTotalSeconds / 60;
  byte tmpHours;
  if (tmpTotalSeconds >= 3600) {
    tmpHours = tmpTotalSeconds / 3600;
  } else {
    tmpHours = 0;
  }

  // print time string unit by unit
  if (tmpHours != lastHours) {
    sprintf(volatileTimeValue, "%02d", tmpHours);
    updateText(volatileTimeValue, xPosVariables, yPosOnline, 2);

  }

  if (tmpMinutes != lastMinutes) {
    sprintf(volatileTimeValue, "%02d", tmpMinutes);
    updateText(volatileTimeValue, xPosVariables + 18, yPosOnline, 2);

  }

  if ((tmpSeconds < 10 || (tmpSeconds % 10 != 0)) && !forcedSecondsRepaint && tmpSeconds != 0) {
    sprintf(volatileTimeValue, "%d", tmpSeconds % 10);
    updateText(volatileTimeValue, xPosVariables + 18 * 2 + 6, yPosOnline, 1);

  } else {
    sprintf(volatileTimeValue, "%02d", tmpSeconds);
    updateText(volatileTimeValue, xPosVariables + 18 * 2, yPosOnline, 2);

    forcedSecondsRepaint = false;
  }

  // save current values for the next time
  lastHours = tmpHours;
  lastMinutes = tmpMinutes;
  lastSeconds = tmpSeconds;
}

void calcFilter(float &value, int newValue, const int filterFaktor) {
  value = ((value * filterFaktor) + newValue) / (filterFaktor + 1.0);
}

void printUpstreamBytes() {
  // filter calculation with factor 1 25% new, 75% old
  calcFilter(currUpstreamFloat, currUpstream, 2);
  currUpstream = int(currUpstreamFloat);

  // avoid printing if possilbe
  if (currUpstream == lastUpstream)
    return;

  // print
  updateVar(int(currUpstreamFloat), xPosVariables, yPosUpstream, 4);

  // save current for the next time and than reset it
  lastUpstream = currUpstream;
  currUpstream = 0;
}

void printTotalBytes() {
  // avoid printing if possilbe
  if (receivedBytes == lastReceivedBytes)
    return;

  // check for bytes, kilobytes or megabytes unit
  if (receivedBytes < 1000) {
    currTotalUnit = BYTES;

    if (currTotalUnit != lastTotalUnit)
      volatileTotalUnit = "Byte";

    // print received bytes
    updateVar(receivedBytes, xPosVariables, yPosTotal, 3);

  } else if (receivedBytes < 1000000) {
    currTotalUnit = KILOBYTES;

    if (currTotalUnit != lastTotalUnit)
      volatileTotalUnit = "KB";

    // print received bytes
    updateVar(receivedBytes/1000, xPosVariables, yPosTotal, 3);

  } else {
    currTotalUnit = MEGABYTES;

    if (currTotalUnit != lastTotalUnit)
      volatileTotalUnit = "MB";

    // print received bytes
    updateVar(receivedBytes/1000000, xPosVariables, yPosTotal, 3);
  }


  if (currTotalUnit != lastTotalUnit || forcedTotalUnitRepaint ) {
      // actuall set the color
    tft.setTextColor(WHITE);
    
    // actual update of the Unit
    updateText(volatileTotalUnit, xPosUnits, yPosTotal, 4);

    // reset Color
    tft.setTextColor(textColors[textColorCurrent]);

    forcedTotalUnitRepaint = false;
  }

  lastTotalUnit = currTotalUnit;
  lastReceivedBytes = receivedBytes;
}

// a function to be executed periodically
void updateStatusBar() {
  printOnlineTime();
  printUpstreamBytes();
  printTotalBytes();
}



