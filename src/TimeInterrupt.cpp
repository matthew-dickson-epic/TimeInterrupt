#include "Arduino.h"
#include "TimeInterrupt.h"

interrupt_list TimeInterrupt;

//Normal base ISR--LOW PRECISION (~976Hz)
ISR(TIMER0_COMPB_vect) 
{
      static unsigned long count;
      timer_interrupt_ *current_interrupt = TimeInterrupt.getCallbacks();
      timer_interrupt_ *previous_interrupt = NULL;
      while(current_interrupt != NULL) //Until end of list is reached
      {
            if (current_interrupt->routine.duration > 0) //If ISR is temporary
            {
                  current_interrupt->routine.calls++; //How many times the function has been acknowledged
                  if (current_interrupt->routine.calls > current_interrupt->routine.duration) //Remove if duration has been met
                  {
                        //-------------------------------------------------------------------------------------------------------------------//
                        //    Removes interrupt by linking previous node's 'next' pointer to current node's next pointer                    //
                        //    thus effectively jumping over the 'current' node. Once the 'current' node has been jumped over,          //
                        //    the memory allocated to the 'current' node is freed. After freeing the memory, the new                        //
                        //    'current' node is set to the next node in the list. The rest of the loop is skipped because                       //
                        //    the rest of the loop justs tries to execute the callback and go to the next interrupt.                               //
                        //----------------------------------------------------------------------------------------------------------------- //

                        if(current_interrupt == TimeInterrupt.getCallbacks())
                        {
                              previous_interrupt = current_interrupt;
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              TimeInterrupt.setHead(current_interrupt);
                              continue;
                        }
                        else
                        {
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              continue;
                        }
                  }
            }   
            if(count % current_interrupt->routine.interval == 0) current_interrupt->routine.callback(); //Call if proper time has elapsed
            previous_interrupt = current_interrupt;
            current_interrupt = current_interrupt->next; //Move to next interrupt in list
      }
      count++;
}

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(OVERRIDE_TIME_INTERRUPT)

ISR(TIMER2_COMPB_vect) 
{
      OCR2B += 250;
      static unsigned long count;
      timer_interrupt_ *current_interrupt = TimeInterrupt.getCallbacks();
      timer_interrupt_ *previous_interrupt = NULL;
      while(current_interrupt != NULL) //Until end of list is reached
      {
            if (current_interrupt->routine.duration > 0) //If ISR is temporary
            {
                  current_interrupt->routine.calls++; //How many times the function has been acknowledged
                  if (current_interrupt->routine.calls > current_interrupt->routine.duration) //Remove if duration has been met
                  {
                        if(current_interrupt == TimeInterrupt.getCallbacks())
                        {
                              previous_interrupt = current_interrupt;
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              TimeInterrupt.setHead(current_interrupt);
                              continue;
                        }
                        else
                        {
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              continue;
                        }
                  }
            }   
            if(count % current_interrupt->routine.interval == 0) current_interrupt->routine.callback(); //Call if proper time has elapsed
            previous_interrupt = current_interrupt;
            current_interrupt = current_interrupt->next; //Move to next interrupt in list
      }
      count++;
}

#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)

ISR(TIMER5_COMPA_vect) 
{
      static unsigned long count;
      timer_interrupt_ *current_interrupt = TimeInterrupt.getCallbacks();
      timer_interrupt_ *previous_interrupt = NULL;
      while(current_interrupt != NULL) //Until end of list is reached
      {
            if (current_interrupt->routine.duration > 0) //If ISR is temporary
            {
                  current_interrupt->routine.calls++; //How many times the function has been acknowledged
                  if (current_interrupt->routine.calls > current_interrupt->routine.duration) //Remove if duration has been met
                  {
                        if(current_interrupt == TimeInterrupt.getCallbacks())
                        {
                              previous_interrupt = current_interrupt;
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              TimeInterrupt.setHead(current_interrupt);
                              continue;
                        }
                        else
                        {
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              continue;
                        }
                  }
            }   
            if(count % current_interrupt->routine.interval == 0) current_interrupt->routine.callback(); //Call if proper time has elapsed
            previous_interrupt = current_interrupt;
            current_interrupt = current_interrupt->next; //Move to next interrupt in list
      }
      count++;
}

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATmega32U4__)

ISR(TIMER1_COMPB_vect) 
{
      OCR1B += 250;
      static unsigned long count;
      timer_interrupt_ *current_interrupt = TimeInterrupt.getCallbacks();
      timer_interrupt_ *previous_interrupt = NULL;
      while(current_interrupt != NULL) //Until end of list is reached
      {
            if (current_interrupt->routine.duration > 0) //If ISR is temporary
            {
                  current_interrupt->routine.calls++; //How many times the function has been acknowledged
                  if (current_interrupt->routine.calls > current_interrupt->routine.duration) //Remove if duration has been met
                  {
                        if(current_interrupt == TimeInterrupt.getCallbacks())
                        {
                              previous_interrupt = current_interrupt;
                              previous_interrupt->next = current_interrupt->next; 
                              free(current_interrupt); //That's a good deal!
                              current_interrupt = previous_interrupt->next;
                              TimeInterrupt.setHead(current_interrupt);
                              continue;
                        }
                        else
                        {
                        previous_interrupt->next = current_interrupt->next; 
                        free(current_interrupt); //That's a good deal!
                        current_interrupt = previous_interrupt->next;
                        continue;
                        }
                  }
            }   
            if(count % current_interrupt->routine.interval == 0) current_interrupt->routine.callback(); //Call if proper time has elapsed
            previous_interrupt = current_interrupt;
            current_interrupt = current_interrupt->next; //Move to next interrupt in list
      }
      count++;
}

#endif
