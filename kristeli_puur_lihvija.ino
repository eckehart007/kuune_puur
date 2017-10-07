#include <Arduino.h>
//#include <avr/wdt.h>

//#define DEBUG

//Pins
int led = 13;
int pwmM2InMinusD3Pin = 3;
int pwmM2InPlusD5Pin = 5;
int buttonDirPlusD11Pin = 11;
int buttonDirMinusD12Pin = 12;
int potPin = A0;

//globals
uint8_t motor_dir = 0;
int avgPotValue = 0;
int motor_pwm_value = 0;
int last_pot_value = 0;

// the setup routine runs once when you press reset:
void setup() {   
  //wdt_disable();             
  // initialize the digital pin as an output.
  //start serial connection
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Startup");
  #endif
  
    //Motor control
  pinMode(pwmM2InMinusD3Pin, OUTPUT);  
  pinMode(pwmM2InPlusD5Pin, OUTPUT); 
  
  analogWrite(pwmM2InMinusD3Pin, 0); //min 50 max 255
  analogWrite(pwmM2InPlusD5Pin, 0);
  
  //Dir button
  pinMode(buttonDirPlusD11Pin, INPUT_PULLUP);
  pinMode(buttonDirMinusD12Pin, INPUT_PULLUP);
  
  //pot
  pinMode(potPin, INPUT_PULLUP);

  last_pot_value = analogRead(potPin);

  //Startup delay
  delay(200);
  //wdt_enable(WDTO_8S);
}

// the loop routine runs over and over again forever:
void loop() {
  avgPotValue =  analogRead(potPin);
  last_pot_value =  avgPotValue;
  #ifdef DEBUG
    Serial.print("last_pot_value : ");                                                                                                                                                                                                                                                                                              
    Serial.println(last_pot_value);
  #endif
  motor_pwm_value = (int)(((float)200 * (float)last_pot_value) / (float)1023)+55;
  
  if ((digitalRead(buttonDirPlusD11Pin) == 0) && (digitalRead(buttonDirMinusD12Pin) == 1)) {
    analogWrite(pwmM2InMinusD3Pin, motor_pwm_value);
    analogWrite(pwmM2InPlusD5Pin, 0);
    motor_dir = 1;
  } else if ((digitalRead(buttonDirMinusD12Pin) == 0) && (digitalRead(buttonDirPlusD11Pin) == 1)) {
    analogWrite(pwmM2InMinusD3Pin, 0);
    analogWrite(pwmM2InPlusD5Pin, motor_pwm_value);
    motor_dir = 2;
  } else {
    motor_dir = 0;
    analogWrite(pwmM2InMinusD3Pin, 0);
    analogWrite(pwmM2InPlusD5Pin, 0);
    delay(200);
  }
  #ifdef DEBUG
    Serial.print("avgPotValue: ");                                                                                                                                                                                                                                                                                              
    Serial.print(avgPotValue);
    Serial.print(" - motor_pwm_value:");
    Serial.print(motor_pwm_value);
    Serial.print(" - motor_dir:");
    Serial.println(motor_dir);
    delay(200);               // wait for a second
   #endif
  delay(10);               // wait for a second
  //wdt_reset();
}
