#include <TimeInterrupt.h>
#define led_pin 3

int countdown = 10;

void setup() 
{
      Serial.begin(115200);
      TimeInterrupt.begin(PRECISION); //Initialize the interrupt with high precision timing
      pinMode(led_pin, OUTPUT);

      TimeInterrupt.addInterrupt(printMessage, 1000, 10000); //Call 'printMessage' every second for 10 seconds
}

void loop() 
{
      digitalWrite(led_pin, HIGH); //Blink LED to show that the interrupt works
      delay(500);
      digitalWrite(led_pin, LOW);
      delay(500);
}

void printMessage()
{
      Serial.print("I'll stop printing in: ");
      Serial.print(countdown);
      Serial.println(" seconds!");
      countdown--; //Decrement the countdown
}
