
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

extern unsigned long screenOffTimeout;

void displaySetup();
void displayLoop();
void displayClear();
void displayPrint(const char text[], int textSize = 1, int x = 0, int y = 0);
void displayShowFor(const char text[], unsigned long ms, int textSize = 1, int x = 0, int y = 0);
void displayLogo();
void displayReady();
bool displayIsOn();
void displayOff();
void displayOn();
