#include "Arduino.h"
#include "InterruptList.h"

interrupt_list::interrupt_list()
{
      this->head = NULL;
      this->tail = NULL;
      this->has_init = false;
}

void interrupt_list::setHead(timer_interrupt_ *new_head)
{
      this->head = new_head;
}

void interrupt_list::begin()
{
      if(this->has_init) //Ignore the function if the interrupt has already been initialized.
            return;

      this->int_precision = NORMAL;
      cli();
      #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      
      this->r.TIMSKn_val = TIMSK; //Store the original register value
      OCR0B = 0;
      TIMSK |= (1<<OCIE0B);

      #else
      this->r.TIMSKn_val = TIMSK0; //Store the original register value
      OCR0B = 0;
      TIMSK0 |= (1<<OCIE0B);
      
      #endif
      sei();
      this->has_init = true;
}

void interrupt_list::begin(bool t)
{
      if(this->has_init) //Ignore the function if the interrupt has already been initialized.
            return;

      this->int_precision = t;
      if(t == PRECISION)
      {
            cli();
            #if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)

            this->r.TCCRnA_val = TCCR5A;
            this->r.TCCRnB_val = TCCR5B;
            this->r.TIMSKn_val = TIMSK5;

            TCCR5A = 0;
            TCCR5B = 0;
            TIMSK5 = 0;
            OCR5A = 249; //Results in 1kHz interrupt
            TCCR5B |= (1<<WGM52); //CTC mode
            TCCR5B |= (1<<CS51) | (1<<CS50); //Prescalar of 64
            TIMSK5 |= (1<<OCIE5A);

            #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)

            this->r.TCCRnA_val = TCCR1A;
            this->r.TIMSKn_val = TIMSK;

            TCCR1 = 0;
            GTCCR |= (1<<PSR1);
            OCR1B = 249; //Results in 1kHz interrupt
            
            if(F_CPU == 16e6)
                  TCCR1 |= (1<<CS12) | (1<<CS11) | (1<<CS10); //Prescalar of 64 (16MHz Clock)
            else if(F_CPU == 8e6)
                  TCCR1 |= (1<<CS12) | (1<<CS11); //Prescalar of 32 (8MHz Clock)
            else
                  TCCR1 |= (1<<CS11) | (1<<CS10); //Prescalar of 4 (1MHz Clock)

            TIMSK |= (1<<OCIE1B);

            #elif defined(__AVR_ATmega32U4__)

            this->r.TCCRnA_val = TCCR1A;
            this->r.TCCRnB_val = TCCR1B;
            this->r.TIMSKn_val = TIMSK1;

            TCCR1A = 0;
            TCCR1B = 0;
            TIMSK3 = 0;
            OCR1A = 249; //Results in 1kHz interrupt
            TCCR1B |= (1<<CS11) | (1<<CS10); //Prescalar of 64
            TIMSK1 |= (1<<OCIE1B);

            #else //ATmega328p/168 precision setup (also serves as default)

            this->r.TCCRnA_val = TCCR2A;
            this->r.TCCRnB_val = TCCR2B;
            this->r.TIMSKn_val = TIMSK2;
            TCCR2A = 0;
            TCCR2B = 0;
            TIMSK2 = 0;
            OCR2B = 249; //Results in 1kHz interrupt

            //Adjust prescaler for clock speed
            if(F_CPU == 16e6)
                  TCCR2B |= (1<<CS22); //Prescalar of 64
            else
                  TCCR2B |= (1<<CS21) | (1<<CS20); //Prescalar of 32
                  
            TIMSK2 |= (1<<OCIE2B);
            #endif
            sei();
      }

      else 
      {
            this->begin();
      }
      this->has_init = true;
}

void interrupt_list::end()
{
      //Restore original register states and change has_init to false (Does not clear the ISR list)
      if(this->has_init == false)
            return;
            
      this->has_init = false;

      if(this->int_precision == PRECISION)
      {
            cli();
            #if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)

            TCCR5A = this->r.TCCRnA_val;
            TCCR5B = this->r.TCCRnB_val;
            TIMSK5 = this->r.TIMSKn_val;

            #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)

            TCCR1A = this->r.TCCRnA_val;
            TIMSK= this->r.TIMSKn_val;

            #elif defined(__AVR_ATmega32U4__)

            TCCR1A = this->r.TCCRnA_val;
            TCCR1B = this->r.TCCRnB_val;
            TIMSK1 = this->r.TIMSKn_val;
            #else

            TCCR2A = this->r.TCCRnA_val;
            TCCR2B = this->r.TCCRnB_val;
            TIMSK2 = this->r.TIMSKn_val;
            #endif
            sei();
      }
      else
      {
            cli();
            #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
            TIMSK = this->r.TIMSKn_val;
            #else
            TIMSK0 = this->r.TIMSKn_val;
            #endif
            sei();
      }
}

void interrupt_list::clear() //Clears all callback functions from ISR list.
{
      timer_interrupt_ *previous_node = NULL;
      timer_interrupt_ *current_node = this->head;
      while (current_node != NULL) //Loop until the list is empty (Cannot clear empty list)
      {
            previous_node = current_node;
            current_node = (current_node->next != NULL)? current_node->next : NULL;
            free(previous_node); //Free the memory of the previous node
      }
      this->head = NULL;
}

bool interrupt_list::getPrecision()
{
      return this->int_precision;
}

timer_interrupt_* interrupt_list::getCallbacks()
{
      return this->head;
}

void interrupt_list::addInterrupt(void (*callback_function)(), unsigned long interval_length, unsigned long duration)
{
      timer_interrupt_ *tmp = new timer_interrupt_;
      tmp->routine.callback = callback_function;
      tmp->routine.interval = interval_length;
      tmp->routine.duration = duration; //Get the call time
      tmp->routine.calls = 0;
      tmp->next = NULL;

      if(this->head == NULL)
      {
            this->head = tmp;
            this->tail = tmp;
      }
      else
      {
            this->tail->next = tmp;
            this->tail = this->tail->next;
      }
}

void interrupt_list::addInterrupt(void (*callback_function)(), unsigned long interval_length)
{
      addInterrupt(callback_function, interval_length, 0); //0 denotes an indefinite call time (will call until removed)
}

void interrupt_list::removeInterrupt(void (*callback_function)())
{
      timer_interrupt_ *previous_node = NULL;
      timer_interrupt_ *current_node = NULL;
      if(this->head->routine.callback == callback_function) //If callback to remove is first item
      {
            current_node = this->head->next; //Store second item in list (could be NULL if only one item)
            free(this->head); //Free the memory containing the first item
            this->head = current_node; //Set the first item to the second item
      }
      else
      {
            previous_node = this->head;
            current_node = this->head->next;
            while(current_node != NULL)//Until end of list
            { 
                  if(current_node->routine.callback == callback_function)
                  {
                        previous_node->next = current_node->next; //Link the previous node to the next node (jump over current)
                        free(current_node); //Free the memory containing the function to remove
                        current_node = previous_node->next; //Set the current node to the node after the removed
                        continue;
                  }
                  previous_node = current_node;
                  current_node = current_node->next;
            }
      }
}