/*
  Blink:  Turns an LED on for one second, then off for one second, repeatedly.

  Default CPU cores:
  - 1 for setup() and loop() functions (running priority 1)
  - 0 for tasks  (default priority 1)
*/
#include <task.h>

  #define LED_TASK               1     // Run as task or periodic loop()
//#define LED_TASK_CORE          1
  #define LED_TASK_STACK_SIZE    configMINIMAL_STACK_SIZE   // Bytes!
  #define tskNORMAL_PRIORITY     1
 

void blink_led( void )
{
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
}


// https://www.freertos.org/a00125.html
void vTaskLedBlink( void * pvParameters )
{
    Serial.print( "LED blinker task started on: Core " );
    Serial.println( xPortGetCoreID() );
    Serial.print( "LED blinker task stack size: " );
    Serial.println( LED_TASK_STACK_SIZE );
    Serial.print( "LED blinker task priority:   ");
    Serial.println( uxTaskPriorityGet(NULL) );
    
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

    // Require delay for PC PlatformIO to open terminal monitor COM port
    delay(3000);
    Serial.begin(115200);
    Serial.print( "LED blinker application started on: Core " );
    Serial.println( xPortGetCoreID() );

    #if    LED_TASK
    #ifdef LED_TASK_CORE
      BaseType_t led_taskCreated = xTaskCreatePinnedToCore (vTaskLedBlink, "blinkLED", LED_TASK_STACK_SIZE, NULL, tskNORMAL_PRIORITY, NULL, LED_TASK_CORE);
    #else
      BaseType_t led_taskCreated = xTaskCreate (vTaskLedBlink, "blinkLED", LED_TASK_STACK_SIZE, NULL, tskNORMAL_PRIORITY, NULL);
    #endif // LED_TASK_CORE
    #endif // LED_TASK
}


// the loop function runs over and over again forever
void loop()
{
   #if !LED_TASK
   blink_led();
   #endif
}
