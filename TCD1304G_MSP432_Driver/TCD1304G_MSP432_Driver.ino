#include "msp.h"
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define OS 13 // Analog input from CCD P5.0 (A5)
#define SH 34 // Shift Gate Pulse, P2.3
#define ICG 38 // Integration Clear Gate pulse, P2.4
#define MCLK 2 // Master clock, synchronize ICG pulse to end on high cycle P6_0
#define DCLK 18 // Data Clock signal to trigger interrupts, P3_0
#define LAMP 56 // PWM signal to lamp, P10.4
// LED1 is the onboard Red LED
// PUSH1 is the left onboard pushbutton

//This is  a line to ensure that this doesn't compile properly until we have the right configuration for the ADC

int data[3694];
int i_time = 145;

void setup() {
  // Halt the Watchdog timer
  MAP_WDT_A_holdTimer();
  /* Initializing ADC (MCLK/1/4) */
  MAP_ADC14_enableModule();
  MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE);
  /* Configuring GPIOs (5.5 A0) */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
  /* Configuring ADC Memory */
  MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A5, false); // false is for non differential Mode
  MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM0, true); // true is for repeat mode
  MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION); // Free-running mode
  MAP_ADC14_setResolution(ADC_14BIT);
  MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_32, ADC_PULSE_WIDTH_32);
  MAP_Interrupt_enableMaster();
  /* Enabling/Toggling Conversion */
  MAP_ADC14_enableConversion();
  MAP_ADC14_toggleConversionTrigger();
  Serial.begin(115200); //230400
  establishContact();
  pinMode(MCLK, INPUT);
  pinMode(DCLK, INPUT);
  pinMode(SH, OUTPUT);
  pinMode(ICG, OUTPUT);
  digitalWrite(SH, LOW);
  digitalWrite(ICG, HIGH);
  pinMode(LAMP, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  digitalWrite(LAMP, LOW);
  delay(500);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  delay(500); 
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(LED1, !digitalRead(LED1));
    char command = char(Serial.read());
    if (command == 'S') {
      initiateScan(i_time);
      readLine();
      sendData();
    }
    if (command == 'L') {
      digitalWrite(LAMP, HIGH);
    }
    if (command == 'F') {
      digitalWrite(LAMP, LOW);
    }
    if (command == 'I') {
      i_time += 5;
    }
    if (command == 'D') {
      i_time -= 5;
      if (i_time < 5) {
        i_time = 5;
      }
    }
  }
  
  if (!digitalRead(PUSH1)) {
    initiateScan(i_time);
    readLine();
    //sendData();
    //digitalWrite(LED1, !digitalRead(LED1));
    //delay(500);
  }
  
 
  initiateScan(i_time);
  delay(i_time);
 // initiateScan(i_time);
 // delay(i_time);
 // initiateScan(i_time);
//  readLine();
 // sendData();
  //digitalWrite(LED1, !digitalRead(LED1));
}

void initiateScan(int i_time) {
  P2OUT = 0B00010000; // Begin the ICG and SH pulses
 delayMicroseconds(8);
  //delayMicroseconds(1);
  P2OUT = 0B00011000; // End the SH pulse
  delayMicroseconds(4);
  P2OUT = 0B00001000; // End the ICG pulse 
  
}

void readLine() {
  for (int i = 0; i < 3694; i++) {
    while (P3IN == 0x0); // Wait for a rising edge of the DCLK
    data[i] = MAP_ADC14_getResult(ADC_MEM0);
    while (P3IN == 0x1);
  }
}

void sendData() {
  for (int i = 0; i < 3694; i++) {
    Serial.write(byte(data[i] >> 8)); // high byte
    Serial.write(byte(data[i])); // low byte
  }
  Serial.flush();
}

void establishContact() {
  Serial.println("Spec");
  delay(20);
  Serial.println("Spec");
}
