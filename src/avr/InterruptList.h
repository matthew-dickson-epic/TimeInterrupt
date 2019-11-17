#ifndef InterruptList_h
#define InterruptList_h

#include "Arduino.h"

#define NORMAL 0x0
#define PRECISION 0x1

struct service_routine_
{
      void (*callback)();
      unsigned long interval;
      unsigned long duration; //0 if perm, >0 if temporary
      unsigned long calls;
};

struct timer_interrupt_
{
      service_routine_ routine;
      timer_interrupt_ *next;
};

struct timer_original_
{
      uint8_t TCCRnA_val;
      uint8_t TCCRnB_val;
      uint8_t TIMSKn_val;
};

class interrupt_list
{
      private:
            bool int_precision;
            bool has_init; //Prevents multiple initializations
            timer_interrupt_ *head, *tail;
            timer_original_ r; //Original register values

      public:
            interrupt_list();
            timer_interrupt_ *getCallbacks();
            void setHead(timer_interrupt_ *new_head);
            void begin();
            void begin(bool t);
            void end(); //Resets the interrupt registers to original states (does not delete callback list)
            void clear(); //Removes all callback functions
            void addInterrupt(void (*callback_function)(), unsigned long interval_length, unsigned long duration);
            void addInterrupt(void (*callback_function)(), unsigned long interval_length);
            void removeInterrupt(void (*callback_function)());
            bool getPrecision();
};

#endif
