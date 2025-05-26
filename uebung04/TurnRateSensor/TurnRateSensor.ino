// TurnRate Sensor: read data from analog gyroscope, output heading on lcd

/* Connections:
 R/S : Pin 11
 R/W : not connected
 E   : Pin 12
 DB4 : Pin 13
 DB5 : Pin A0
 DB6 : Pin A1
 DB7 : Pin A2
 
 Motor:
 A1 : Pin 5
 A2 : Pin 6
 B1 : Pin 9
 B2 : Pin 10
 Turn-Rate Sensor: A3
*/
// include LCD functions:
#include <LiquidCrystal.h> 

// define the LCD screen
LiquidCrystal lcd(11, 12, 13, A0, A1, A2);

// Pins of motor
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_A2 6
#define PIN_MOTOR_B1 9
#define PIN_MOTOR_B2 10
const uint8_t motorPins[] = { PIN_MOTOR_A1, PIN_MOTOR_A2, PIN_MOTOR_B1, PIN_MOTOR_B2 };

// initialization
void setup()
{
   // LCD has 4 lines with 20 chars
   lcd.begin(20, 4); 
   lcd.print("system ready");
  
  
}

void loop()
{  
   // read the current analog value on a3
   int16_t analogValue = analogRead(A3);
   // implement your code here:
   
}


/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/

