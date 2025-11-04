/*
                    LAB_11_1002.ino
Cael Link
29 October 2024
ECE 1002-002 
Lab 11 Temp n light

Problem: Use an lcd to display if the tempurature is increasing or stable for 10 seconds then a
reading of the light percentage from 0 to 100 using the map function. 


Algorithm

pin variables

temp task variables
light variables

setup {
  begin lcd
  input for senors, button
}

loop {
  if not calibrated
  calibrate
  else
  while time <= 10000ms
  call task 2

  clear lcd
  reset time

  while time <=10000ms
  run task 1

}

task 1() {
  convert to C
  calc avg
  print 3 values across top row

  if avg > prev
  rising
  elif avg< prev
  falling
  else
  stable

  move readings down to previous
delay 750
}

calibrate() {
  if button pressed
  if dark flag- true
  set dark variable
  }
  else {
    set light variable
    set calibrated flag to true
  }

  }
  
}

task 2 {

  read light value
  calibrate = map(lightValue)
  if over or under limit set back to 0 and 100

  print percentage of light
  delay 750
}
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

//pins
const int tempPin = A0;
const int lightPin = A1;
const int buttonPin = 1;

//temp variables
float voltage= 0.0;
float tempCurrent = 0.0;
float tempPrev1 = 0.0;
float tempPrev2 = 0.0;
float avgCurrent = 0.0;
float avgPrev = 0.0;
float threshold = 0.15;

//light variables
int lightValue = 0;
int lightest = 0;
int darkest = 0;
int calibratedLight = 0;
bool flag = true;
bool isCalibrated = false;

//timer
const int taskDuration = 10000;  // Duration for each task (10 seconds)


void setup() {

  lcd.begin(16, 2);      
  pinMode(tempPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
}

void loop() {
if (isCalibrated == false) {
    calibrate();
  } else {
    //starttime for when else begins
    unsigned int startTime = millis();

    // Run task2 (light reading) for 10 seconds
    while (millis() - startTime < taskDuration) {
      task2();
    }
    lcd.clear();

    // Run task1 (temperature reading) for 10 seconds
    startTime = millis();
    while (millis() - startTime < taskDuration) {
      task1();
    }

    // Clear display before switching back to task2
    lcd.clear();
  }
}


void task1() {

  voltage = analogRead(A0) * 4.8828125;
  tempCurrent = (voltage - 500)/10;
  avgCurrent = (tempCurrent + tempPrev1 + tempPrev2) / 3;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(tempCurrent,1);
  lcd.setCursor(6, 0);
  lcd.print(tempPrev1, 1);
  lcd.setCursor(12, 0);
  lcd.print(tempPrev2, 1);


  lcd.setCursor(0, 1);   // Set cursor to the lower row
  if (avgCurrent >= avgPrev + threshold) {
    lcd.print("rising");  
    lcd.print("");
  } else if (avgCurrent <= avgPrev - threshold) {
    lcd.print("         falling");
    lcd.print("");
  } else {
    lcd.print("     stable");
    lcd.print("");
  }


  tempPrev2 = tempPrev1;
  tempPrev1 = tempCurrent;
  avgPrev = avgCurrent;
  delay(750);

}

void calibrate() {
  lcd.setCursor(0,0);
  lcd.print("Push to calibrate");
  if (digitalRead(buttonPin) == LOW) {
    if(flag) {
    darkest = analogRead(lightPin);
    delay(100);
    lcd.clear();
    lcd.print(darkest);
    delay(500);
    flag = false;
    } else { 
      lightest = analogRead(lightPin);
      delay(100);
      lcd.clear();
      lcd.print(lightest);
      delay(500);
      lcd.clear();
      lcd.print("Calibrated");
      isCalibrated = true;

    }
  }
}
void task2() {
  lightValue = analogRead(lightPin);
  calibratedLight = map(lightValue, darkest, lightest, 0, 100);

  if(calibratedLight < 0) {
    calibratedLight = 0;
  } else if (calibratedLight > 100) {
    calibratedLight = 100;
  }

  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(calibratedLight);
  lcd.print("%");

  delay(750); 

}