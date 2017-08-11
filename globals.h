#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TAB_LENGTH 6
const int TAB_RASTER = TAB_LENGTH * 6;

byte textColorCurrent = 0;
byte textColorNum = 5;
unsigned int textColors[] = {
  WHITE,
  GREEN,
  CYAN,
  YELLOW,
  RED
};

byte baudRateCurrent = 2;
byte baudRatesNum = 5;
long baudRates[] = {
  300,
  2400,
  9600,
  57600,
  115200
};

// 320 * 480 is 60 lines with 8 pixels
int16_t lineHeight = 8;
int16_t topOffset = 8 * lineHeight;
int16_t lines = 52;
int16_t scroll = 0;
int16_t cursorX = 0;
int16_t cursorY = topOffset;

boolean isPageFull = false;
int cursorPositionsList[52]; 
byte cursorPositionsCounter = 0;

long receivedBytes = 0;
long lastReceivedBytes = -1;
bool forcedTotalUnitRepaint = false;

byte lastMinutes = -1;
byte lastHours = -1;
byte lastSeconds = -1;
bool forcedSecondsRepaint = false;

enum {BYTES = 0, KILOBYTES, MEGABYTES};
byte currTotalUnit = BYTES;
byte lastTotalUnit = -1;

long currUpstream = 0;
long lastUpstream = -1;
float currUpstreamFloat;

