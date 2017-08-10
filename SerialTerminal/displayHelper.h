#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void initDisplay() {
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(0);     //Portrait
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);

  tft.setTextSize(2);
  tft.setCursor(70, 8);
  tft.print("Serial");

  tft.setCursor(70, 32);
  tft.print("Terminal");
  tft.setTextSize(1);

  tft.setCursor(180, 8);
  tft.print("Speed:");
  tft.setCursor(295, 8);
  tft.print("Baud");

  tft.setCursor(180, 24);
  tft.print("Timer:");
  tft.setCursor(295, 24);
  tft.print("s");

  tft.setCursor(180, 40);
  tft.print("Received:");
  tft.setCursor(295, 40);
  tft.print("Byte");
}

void writeToDisplay(int x, int y, long var) { 
  // clear space for the new line
  tft.fillRect(x, y, 45, 8, BLACK);
  tft.setCursor(x, y);
  tft.print(var, DEC);
}


