#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

#define OLED_RESET     -1 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum PinAssignments {
  encoderPinA = 2, // ขา S1
  encoderPinB = 3, // ขา S2
  clearButton = 4  // ขา Key
                // จ่ายไฟที่ขา +5v และ gnd  
                // Example For Arduino Uno By www.ArduinoAll.com
};

volatile int encoderPos = 0;  // สำหรับนับจำนวน
int lastReportedPos = 1;   // change management
static boolean rotating = false;    // debounce management

boolean A_set = false;
boolean B_set = false;

char set[3][8] = {'keki1', 'keki2', 'keki3'};


// คำสั่งทำงานแบบ interrupt เมื่อมีการหมุน
void doEncoderA() {
  // debounce
  if ( rotating ) delay (5);  // หน่วงเวลาป้องกันสัญญาณบกวน debounce

  // เช็คว่ามีบิดสวิตช์
  if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
    A_set = !A_set;
    // adjust counter + if A leads B
    if ( A_set && !B_set )
      encoderPos += 1;
    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if ( rotating ) delay (5);
  if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set )
      encoderPos -= 1;
    rotating = false;
  }
}

void setup() {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);

  pinMode(encoderPinA, INPUT_PULLUP); // กำหนดโหมดเป็นแบบ Input pullup
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(clearButton, INPUT_PULLUP);

  attachInterrupt(0, doEncoderA, CHANGE); //ทำงานแบบ interrupt เบอร์ 0 ในนี้คือขา pin 2
  attachInterrupt(1, doEncoderB, CHANGE); //ทำงานแบบ interrupt เบอร์ 1 ในนี้คือขา pin 3
}

void loop() {
  rotating = true;  // reset the debouncer

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK); 
  OLED.setTextSize(2); 

  OLED.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  
  OLED.setCursor(SCREEN_WIDTH/2 - 6, SCREEN_HEIGHT/2 - 8);

  if (lastReportedPos != encoderPos) {
    Serial.print("Index:");
    Serial.println(encoderPos);
    
    lastReportedPos = encoderPos;
  }
  if (digitalRead(clearButton) == LOW )  {
    encoderPos = 0;
    Serial.println("Click:Reset Index");
    delay(200);
  }
  
  if (encoderPos < 0 ){
    if (encoderPos <= -26 || encoderPos >= 26) encoderPos = 0;
    OLED.println(set[26 - (abs(encoderPos) % 26)]);
  }else{
    OLED.println(set[encoderPos % 26]);      
  }
  OLED.display();
}

// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128 
// #define SCREEN_HEIGHT 32 

// #define OLED_RESET     -1 
// Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// enum PinAssignments {
//   encoderPinA = 2, // ขา S1
//   encoderPinB = 3, // ขา S2
//   clearButton = 4  // ขา Key
//                 // จ่ายไฟที่ขา +5v และ gnd  
//                 // Example For Arduino Uno By www.ArduinoAll.com
// };

// volatile int encoderPos = 0;  // สำหรับนับจำนวน
// int lastReportedPos = 1;   // change management
// static boolean rotating = false;    // debounce management

// boolean A_set = false;
// boolean B_set = false;

// char set[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


// // คำสั่งทำงานแบบ interrupt เมื่อมีการหมุน
// void doEncoderA() {
//   // debounce
//   if ( rotating ) delay (5);  // หน่วงเวลาป้องกันสัญญาณบกวน debounce

//   // เช็คว่ามีบิดสวิตช์
//   if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
//     A_set = !A_set;
//     // adjust counter + if A leads B
//     if ( A_set && !B_set )
//       encoderPos += 1;
//     rotating = false;  // no more debouncing until loop() hits again
//   }
// }

// // Interrupt on B changing state, same as A above
// void doEncoderB() {
//   if ( rotating ) delay (5);
//   if ( digitalRead(encoderPinB) != B_set ) {
//     B_set = !B_set;
//     //  adjust counter - 1 if B leads A
//     if ( B_set && !A_set )
//       encoderPos -= 1;
//     rotating = false;
//   }
// }

// void setup() {
//   OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//   Serial.begin(9600);

//   pinMode(encoderPinA, INPUT_PULLUP); // กำหนดโหมดเป็นแบบ Input pullup
//   pinMode(encoderPinB, INPUT_PULLUP);
//   pinMode(clearButton, INPUT_PULLUP);

//   attachInterrupt(0, doEncoderA, CHANGE); //ทำงานแบบ interrupt เบอร์ 0 ในนี้คือขา pin 2
//   attachInterrupt(1, doEncoderB, CHANGE); //ทำงานแบบ interrupt เบอร์ 1 ในนี้คือขา pin 3
// }

// void loop() {
//   rotating = true;  // reset the debouncer

//   OLED.clearDisplay();
//   OLED.setTextColor(WHITE, BLACK); 
//   OLED.setTextSize(2); 

//   OLED.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  
//   OLED.setCursor(SCREEN_WIDTH/2 - 6, SCREEN_HEIGHT/2 - 8);

//   if (lastReportedPos != encoderPos) {
//     Serial.print("Index:");
//     Serial.println(encoderPos);
    
//     lastReportedPos = encoderPos;
//   }
//   if (digitalRead(clearButton) == LOW )  {
//     encoderPos = 0;
//     Serial.println("Click:Reset Index");
//     delay(200);
//   }
  
//   if (encoderPos < 0 ){
//     if (encoderPos <= -26 || encoderPos >= 26) encoderPos = 0;
//     OLED.println(set[26 - (abs(encoderPos) % 26)]);
//   }else{
//     OLED.println(set[encoderPos % 26]);      
//   }
//   OLED.display();
// }
