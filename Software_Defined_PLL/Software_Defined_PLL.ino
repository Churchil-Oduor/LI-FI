#include "main.h"
#define CARRIER_FREQUENCY 10000
#define PI 3.141592654
#define omega 2 * PI * CARRIER_FREQUENCY
#define SAMPLING_FREQUENCY 200000

float dc_offset = 0.9998;
float step = 1.0;
float received_signal = 0.0;
float oscillator_signal = 0.0;
float angle = 0.0;
float previous_signal = 0.0;
float mixed_signal = 0.0;
void setup()
{
  Serial.begin(9600);
}


void loop()
{
  angle = omega * step;
  received_signal = sin(angle);//SAMPLING_FREQUENCY);
  oscillator_signal = sin(angle);//) + acos(dc_offset));
  mixed_signal = received_signal * oscillator_signal;
  dc_offset = average_shift(mixed_signal, previous_signal);
  previous_signal = mixed_signal;
  step++;
  Serial.println(dc_offset);
}
