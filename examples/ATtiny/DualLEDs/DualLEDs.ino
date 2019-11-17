#include <TimeInterrupt.h>
#define del_LED 3
#define int_LED 4

void setup() 
{
      TimeInterrupt.begin(PRECISION); //Initialize the interrupt with high precision timing
      pinMode(del_LED, OUTPUT);
      pinMode(int_LED, OUTPUT);

      TimeInterrupt.addInterrupt(printMessage, 1000); //Call 'printMessage' every second
}

void loop() 
{
      digitalWrite(del_LED, HIGH); //Blink LED to show that the interrupt works
      delay(500);
      digitalWrite(del_LED, LOW);
      delay(500);
}

void toggleLed()
{
      digitalWrite(int_LED, !digitalRead(int_LED));
}
