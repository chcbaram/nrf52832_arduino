/*
  Pedometer.ino

  Written by Jeff Wu (jeff@arduino.org)

  This example for the Arduino Primo Core shows how to use
  CoreSensors library.
  The example is to show how to count steps with the Primo Core.  
  Trying to simulate the steps, when new step detected, the 
  USER LED will blink once.
  You can check data using serial or BLE *nRF Toolbox* 
  uart application. When a device with NFC is near to the 
  Primo Core with NFC antenna, it will try to open the 
  Nordic's *nRF Toolbox* or look for the app in the store.

  This example code is in the public domain.

*/

#include <CoreSensors.h>
#include <BLEPeripheral.h>
#include <BLESerial.h>
#include <NFC.h>

// interrupt pin
#define INT1 21

// create ble serial instance
BLESerial bleSerial = BLESerial();

// specify the package name for windows and android phone and insert the EOL character at the end '\0'
static const char android_package_name[] = {'n', 'o', '.', 'n', 'o', 'r', 'd', 'i', 'c', 's',
                                               'e', 'm', 'i', '.', 'a', 'n', 'd', 'r', 'o', 'i', 
                                               'd', '.', 'n', 'r', 'f', 't', 'o', 'o', 'l', 'b',
                                               'o', 'x','\0'};

static const char windows_application_id[] = {'{', 'e', '1', '2', 'd', '2', 'd', 'a', '7', '-',
                                                 '4', '8', '8', '5', '-', '4', '0', '0', 'f', '-',
                                                 'b', 'c', 'd', '4', '-', '6', 'c', 'b', 'd', '5',
                                                 'b', '8', 'c', 'f', '6', '2', 'c', '}', '\0'};

int step = 0;

uint32_t previous_tick = 0;
uint32_t current_tick = 0;
uint16_t step_count = 0;
char report[256];

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial port at 115200 bps
  Serial.begin(115200);
  // custom services and characteristics can be added as well
  bleSerial.setLocalName("Arduino Primo Core");
  // start ble serial
  bleSerial.begin();
  // set the Android packge name as first and the Windows application id as second
  NFC.setAPPmessage(android_package_name, windows_application_id);
  // start the NFC module
  NFC.start();
  // start the core sensors
  coresensors.begin();
  // enable pedometer
  coresensors.enablePedometer();
  // set up interrupt pin to sensor
  attachInterrupt(INT1, Step, RISING);
  previous_tick = millis();
}

void loop() {
  // check interrupt flag
  if (step)
  {
    step = 0;
    uint8_t status = 0;
    // get pedometer status
    coresensors.getStatusPedometer(status);
    if (status)
    {
      // New step detected, print the step counter
      coresensors.getStepCounter(step_count);
      snprintf(report, sizeof(report), "Step counter: %d", step_count);
      Serial.println(report);
      bleSerial.println(report);
      
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
  // Print the step counter every 3000 ms
  current_tick = millis();
  if((current_tick - previous_tick) >= 3000)
  {
    coresensors.getStepCounter(step_count);
    snprintf(report, sizeof(report), "Step counter: %d", step_count);
    Serial.println(report);
    bleSerial.println(report);
    previous_tick = millis();
  }
  
}

// interrupt service routine
void Step()
{
  step = 1;
}