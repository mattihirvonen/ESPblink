/*
  Blink:  Turns an LED on for one second, then off for one second, repeatedly.
*/
#include <task.h>

  #define SERIAL_PRINT     0
  #define LED_TASK         0
//#define LED_SERVER_CORE  1

  #define LED_TASK_STACK_SIZE    100    // words not bytes
  #define tskNORMAL_PRIORITY     1


void blink_led( void )
{
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
    Serial.println("LED blink");
}


// https://www.freertos.org/a00125.html
void vTaskLedBlink( void * pvParameters )
{
    #if SERIAL_PRINT
    Serial.println("LED blinker task started");
    #endif
    
    while ( 1 )
    {
        blink_led();
    }
}


// the setup function runs once when you press reset or power the board
void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    #if SERIAL_PRINT
    Serial.begin (115200);
    Serial.println("LED blinker application started");
    #endif

    #if    LED_TASK
    #ifdef LED_SERVER_CORE
      BaseType_t led_taskCreated = xTaskCreatePinnedToCore (vTaskLedBlink, "blinkLED", LED_TASK_STACK_SIZE, NULL, tskNORMAL_PRIORITY, NULL, LED_SERVER_CORE);
    #else
      BaseType_t led_taskCreated = xTaskCreate (vTaskLedBlink, "blinkLED", LED_TASK_STACK_SIZE, NULL, tskNORMAL_PRIORITY, NULL);
    #endif // LED_SERVER_CORE
    #endif // LED_TASK
}


// the loop function runs over and over again forever
void loop()
{
   #if !LED_TASK
   blink_led();
   #endif
}
