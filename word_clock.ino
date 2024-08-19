#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <FastLED.h>
#include <Timezone.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define LED_PIN D6
#define NUM_LEDS 124 // not sure
#define BRIGHTNESS 20

WiFiUDP ntpUDP;

// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);

NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // do not change offset

int period = 1000; //Update frequency
unsigned long time_now = 0;
int Seconds, Minute, Hour;

CRGB leds[NUM_LEDS];
CHSV colour(100, 0, 255);

// the first element is the length of the array
int WordIt[] = {2, 0, 1};
int WordIs[] = {2, 2, 3};
int WordHalf[] = {4, 4, 5, 6, 7};
int WordA[] = {1, 8};
int WordTwenty[] = {6, 13, 14, 15, 16, 17, 18};
int WordMinFive[] = {4, 9, 10, 11, 12};
int WordMinTen[] = {3, 19, 20, 21};
int WordQuarter[] = {7, 22, 23, 24, 25, 26, 27, 28};
int WordMinutes[] = {7, 31, 32, 33, 34, 35, 36, 37};
int WordTo[] = {2, 29, 30};
int WordPast[] = {4, 38, 39, 40, 41};
int WordOClock[] = {6, 101, 102, 103, 104, 105, 106};
int WordIn[] = {2, 99, 100};
int WordThe[] = {3, 107, 108, 109};
int WordMorning[] = {7, 110, 111, 112, 113, 114, 115, 116};
int WordEvening[] = {8, 117, 118, 119, 120, 121, 122, 123, 124};

// arr[rows][cols]
const int WordNumbers[13][10] = {
    {0},                             // Zero should do nothing
    {3, 56, 55, 54},                 // One
    {3, 53, 52, 51},                 // Two
    {5, 42, 43, 44, 45, 46},         // Three
    {4, 47, 48, 49, 50},             // Four
    {4, 63, 64, 65, 66},             // Five
    {3, 87, 86, 85},                 // Six
    {5, 77, 78, 79, 80, 81},         // Seven
    {5, 92, 91, 90, 89, 88},         // Eight
    {4, 76, 75, 74, 73},             // Nine
    {3, 82, 83, 84},                 // Ten
    {6, 57, 58, 59, 60, 61, 62, 63}, // Eleven
    {6, 93, 94, 95, 96, 97, 98}      // Twelve
};

// hour is in 24
void adjustBrightness(int currentHour)
{
  if (currentHour >= 11 && currentHour <= 16)
  {
    FastLED.setBrightness(60);
  } else {
    FastLED.setBrightness(30);
  }
}

void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  clearAll();
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  bool res;
  res = wm.autoConnect(); // auto generated AP name from chipid
  
  if(!res) {
    Serial.println("Failed to connect");
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.print("Wifi connected");
  }
    
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  timeClient.begin();
  delay(10);
}

void clearAll()
{
  FastLED.clear();
  FastLED.show();
}

void displayWord(const int WordArr[])
{
  // WordArr[0] contains each respective array length
  // start at WordArr[1]
  for (int i = 1; i <= WordArr[0]; i++)
  {
    leds[WordArr[i]] = colour;
    Serial.print(WordArr[i]);
    Serial.print(",");
  }
  Serial.print("\n");
}

void displayAllWords()
{
  displayWord(WordIt);
  displayWord(WordIs);
  displayWord(WordHalf);
  displayWord(WordA);
  displayWord(WordTwenty);
  displayWord(WordMinFive);
  displayWord(WordMinTen);
  displayWord(WordQuarter);
  displayWord(WordMinutes);
  displayWord(WordTo);
  displayWord(WordPast);
  displayWord(WordOClock);
  displayWord(WordIn);
  displayWord(WordThe);
  displayWord(WordMorning);
  displayWord(WordEvening);
  displayWord(WordNumbers[1]);
  displayWord(WordNumbers[2]);
  displayWord(WordNumbers[3]);
  displayWord(WordNumbers[4]);
  displayWord(WordNumbers[5]);
  displayWord(WordNumbers[6]);
  displayWord(WordNumbers[7]);
  displayWord(WordNumbers[8]);
  displayWord(WordNumbers[9]);
  displayWord(WordNumbers[10]);
  displayWord(WordNumbers[11]);
  displayWord(WordNumbers[12]);
  FastLED.show();
}

void displayHourRoundUp(int Hour)
{
  if (Hour == 12)
  {
    displayWord(WordNumbers[1]);
     Serial.println(1);
  }
  else
  {
    displayWord(WordNumbers[Hour + 1]);
     Serial.println(Hour+1);
  }
}

void displayHourRoundDown(int Hour)
{
  displayWord(WordNumbers[Hour]);
  Serial.println(Hour);
}

void loop()
{
  // put your main code here, to run repeatedly:
  timeClient.update();
  int Hours;
  unsigned long unix_epoch = timeClient.getEpochTime(); // get UNIX Epoch time
  unsigned long adjusted_unix_epoch = usPT.toLocal(unix_epoch);
  Seconds = second(adjusted_unix_epoch); // get Secondss
  Minute = minute(adjusted_unix_epoch);  // get minutes
  Hours = hour(adjusted_unix_epoch);     // get hours
  adjustBrightness(Hours);

  // Clear FastLED to remove ones that don"t need to be lit
  FastLED.clear();
  
  if (Hours > 12)
  {
    Hour = Hours - 12;
  }
  else
    Hour = Hours;

  Serial.print(Hour);
  Serial.print(":");
  Serial.print(Minute);
  Serial.print("\n");

  // words to always display
  displayWord(WordIt);
  displayWord(WordIs);
  displayWord(WordOClock);

  //     it is five minutes past
  if ((Minute >= 5) && (Minute < 10))
  {
     Serial.println("it is five minutes past");
    displayWord(WordMinFive);
    displayWord(WordMinutes);
  }
  // it is ten minutes past
  else if ((Minute >= 10) && (Minute < 15))
  {
     Serial.println("it is ten minutes past");
    displayWord(WordMinTen);
    displayWord(WordMinutes);
  }
  // it is a quarter past
  else if ((Minute >= 15) && (Minute < 20))
  {
     Serial.println("it is a quarter past");
    displayWord(WordA);
    displayWord(WordQuarter);
  }
  // it is twenty minutes past
  else if ((Minute >= 20) && (Minute < 25))
  {
     Serial.println("it is twenty minutes past");
    displayWord(WordTwenty);
    displayWord(WordMinutes);
  }
  // it is twenty five minutes past
  else if ((Minute >= 25) && (Minute < 30))
  {
     Serial.println("it is twenty five minutes past");
    displayWord(WordTwenty);
    displayWord(WordMinFive);
    displayWord(WordMinutes);
  }
  // it is half past
  else if ((Minute >= 30) && (Minute < 35))
  {
     Serial.println("it is half past");
    displayWord(WordHalf);
  }
  // it is twenty five minutes to
  else if ((Minute >= 35) && (Minute < 40))
  {
     Serial.println("it is twenty five minutes to");
    displayWord(WordTwenty);
    displayWord(WordMinFive);
    displayWord(WordMinutes);
  }
  // it is twenty minutes to
  else if ((Minute >= 40) && (Minute < 45))
  {
     Serial.println("it is twenty minutes to");
    displayWord(WordTwenty);
    displayWord(WordMinutes);
  }
  // it is a quarter to
  else if ((Minute >= 45) && (Minute < 50))
  {
     Serial.println("it is a quarter to");
    displayWord(WordA);
    displayWord(WordQuarter);
  }
  // it is ten minutes to
  else if ((Minute >= 50) && (Minute < 55))
  {
     Serial.println("it is ten minutes to");
    displayWord(WordMinTen);
    displayWord(WordMinutes);
  }
  // it is five minutes to
  else if ((Minute >= 55) && (Minute <= 59))
  {
    Serial.println("it is five minutes to");
    displayWord(WordMinFive);
    displayWord(WordMinutes);
  } else {
    Serial.println("error: somehow minute is not within 0 to 60 range");
  }

  if ((Minute < 35) && (Minute >= 5))
  {
    displayWord(WordPast);
    displayHourRoundDown(Hour);
  }
  else
  {
    displayWord(WordTo);
    displayHourRoundUp(Hour);
  }
  
  
  if ((Hours > 19) && (Hours < 21))
  {
    displayWord(WordIn);
    displayWord(WordThe);
    displayWord(WordEvening);
  }
  else if (Hours < 12)
  {
    displayWord(WordIn);
    displayWord(WordThe);
    displayWord(WordMorning);
  }

  FastLED.show();
}
