#include <Arduino.h>
#include <TMC2209.h>

const long SERIAL_BAUD_RATE = 115200;
const uint8_t RUN_CURRENT_PERCENT = 100;

// Instantiate TMC2209
TMC2209 stepper_driver[4];
const TMC2209::SerialAddress SERIAL_ADDRESS[4] = {TMC2209::SERIAL_ADDRESS_0,
    TMC2209::SERIAL_ADDRESS_1,
    TMC2209::SERIAL_ADDRESS_2,
    TMC2209::SERIAL_ADDRESS_3};

bool invert_direction[4] = {false};
bool r = false;

int ME[6]= { 22, 48, 8, 9, 10, 11 };
int MS[6]= { 26, 52, 39, 3, 5, 7 };
int MD[6]= { 24, 50, 37, 2, 4, 6 };

void setup()
{
  int i = 0;

  for (i=0; i<6; i++) {
    pinMode(ME[i], OUTPUT);
    pinMode(MS[i], OUTPUT);
    pinMode(MD[i], OUTPUT);
    digitalWrite(ME[i], LOW);
    digitalWrite(MS[i], HIGH);
    digitalWrite(MD[i], HIGH);
  }

  Serial.begin(SERIAL_BAUD_RATE);

  for (i = 0; i < 4; i++){
    stepper_driver[i].setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS[i]);
    if (stepper_driver[i].isSetupAndCommunicating())
    {
      Serial.print("Stepper driver ");
      Serial.print(i);
      Serial.println(" setup and communicating!");
    }
    else
    {
      Serial.print("Stepper driver ");
      Serial.print(i);
      Serial.println(" Not setup and communicating!");
      continue;
    }
    stepper_driver[i].moveUsingStepDirInterface();
    stepper_driver[i].setMicrostepsPerStepPowerOfTwo(4);
    stepper_driver[i].setRunCurrent(RUN_CURRENT_PERCENT);
    stepper_driver[i].enableCoolStep();
    stepper_driver[i].enable();
  }

}

void loop()
{
  int i = 0, d = 0, s = 0;

  if (r) {
    // USE this for CLOCK WISE
    for (d = 0; d < 2; d++)
      digitalWrite(MD[d], LOW);
    for (d = 2; d < 6; d++)
      digitalWrite(MD[d], HIGH);

    for (i=0; i<200*16; i++) {
      for (s = 0; s < 6; s++)
        digitalWrite(MS[s], HIGH);
      delayMicroseconds(300);
      for (s = 0; s < 6; s++)
        digitalWrite(MS[s], LOW);
      delayMicroseconds(300);
      // change the delays for change in speed
    }
  } else {
    // USE this for ANTI-CLOCK WISE
    for (d = 0; d < 2; d++)
      digitalWrite(MD[d], HIGH);
    for (d = 2; d < 6; d++)
      digitalWrite(MD[d], LOW);

    for (i=0; i<200*16; i++) {
      for (s = 0; s < 6; s++)
        digitalWrite(MS[s], HIGH);
      delayMicroseconds(300);
      for (s = 0; s < 6; s++)
        digitalWrite(MS[s], LOW);
      delayMicroseconds(300);
      // change the delays for change in speed
    }
  }

  if (r)
    r = false;
  else
    r = true;

  delay(1000);
}
