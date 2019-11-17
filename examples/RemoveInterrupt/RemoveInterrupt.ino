#include <TimeInterrupt.h>
#define led_pin 3

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
}

void printMessage()
{
      Serial.println("Type any character to stop printing");
}

void serialEvent()
{
      if(Serial.available() > 0)
      {
            while(Serial.available()) //Flush the input FIFO
                  Serial.read();
                  
            TimeInterrupt.removeInterrupt(printMessage);
      }
}
