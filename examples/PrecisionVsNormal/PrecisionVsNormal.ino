#include <TimeInterrupt.h>
#define led_pin 3

int countdown = 10;

void setup() 
{
      Serial.begin(115200);
      TimeInterrupt.begin(PRECISION); //Initialize the interrupt with high precision timing
      pinMode(led_pin, OUTPUT);

      TimeInterrupt.addInterrupt(printMessage, 1000); //Call 'printMessage' every second
}

void loop() 
{
      digitalWrite(led_pin, HIGH); //Blink LED to show that the interrupt works
      delay(500);
      digitalWrite(led_pin, LOW);
      delay(500);
      if(countdown > 0)
            countdown--;
      else if(countdown == 0)
      {
            countdown = -1;
            Serial.println("Switching to low precision");
            TimeInterrupt.end(); //End the interrupt so it can be re-initialized
            TimeInterrupt.begin(NORMAL); //Initialize the interrupt with low precision
      }
}

void printMessage()
{
      Serial.print("Elapsed milliseconds: ");
      Serial.println(millis());
}
