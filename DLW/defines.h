// Pin assignments for the Arduino (Make changes to these if you use different Pins)
#define BACKLIGHT 10                      // Pin used for the LCD Backlight
#define SDSSPIN 53                        // SD card CS pin
#define NPPIN 49                          // Data Pin for the NeoPixel LED Strip
#define AuxButton 44                      // Aux Select Button Pin
#define AuxButtonGND 45                   // Aux Select Button Ground Pin

//LED-strip variables
#define STRIP_LENGTH 288                  // Set the number of LEDs the LED Strip
int g = 0;                                // Variable for the Green Value
int b = 0;                                // Variable for the Blue Value
int r = 0;                                // Variable for the Red Value

// Intial Variable declarations and assignments (Make changes to these if you want to change defaults)
int frameDelay = 1 ;                      // default for the frame delay
int menuItem = 1;                         // Variable for current main menu selection
int initDelay = 0;                        // Variable for delay between button press and start of light sequence
int repeat = 0;                           // Variable to select auto repeat (until select button is pressed again)
int repeatDelay = 0;                      // Variable for delay between repeats
int updateMode = 0;                       // Variable to keep track of update Modes
int repeatTimes = 1;                      // Variable to keep track of number of repeats
int brightness = 100;                     // Variable and default for the Brightness of the strip

byte x;

// BacklightControl to save battery Life
#define LCD_BRIGHTNESS 100                // LCD Brightness
#define BackLightTimeout 500              // Adjust this to a larger number if you want a longer delay
boolean BackLightTimer = false;             
int BackLightTemp =  BackLightTimeout;

// Variable assignments for the Keypad
int adc_key_val[5] = { 30, 170, 390, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -1;
bool menu_updated = true;                 // Variable to determine if menu needs to be updated
enum keys {RIGHT, UP, DOWN, LEFT, SELECT};

// SD Card Variables and assignments
File root;
File dataFile;
String m_CurrentFilename = "";
int m_FileIndex = 0;
int m_NumberOfFiles = 0;
String m_FileNames[200];
