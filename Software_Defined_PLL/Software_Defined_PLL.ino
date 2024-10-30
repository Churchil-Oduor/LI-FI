#include "main.h"
#define CARRIER_FREQUENCY 100000
#define PI 3.141592654
#define OMEGA 2 * PI * CARRIER_FREQUENCY
#define SAMPLING_FREQUENCY 300000
#define ALPHA 0.019

double offset_angle = 0.0;
double step = 1.0;
double received_signal = 0.0;
double oscillator_signal = 0.0;
double previous_signal = 0.0;
double angle = 0.0;
double mixed_signal = 0.0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  for (step = 0; step < SAMPLING_FREQUENCY; step++)
  {
      angle = OMEGA * step / SAMPLING_FREQUENCY;
      received_signal = cos(angle);
      oscillator_signal = cos(angle);
      mixed_signal = received_signal * oscillator_signal;
      offset_angle = lowPass_filter(mixed_signal, previous_signal);
      previous_signal = offset_angle;  
      Serial.println(acos(offset_angle * 2) * 180 / PI);  
  }
}


/**
 * lowPass_filter - removes the high frequency signal after the mixing
 * process.
 *
 * @mixed_signal: the signal as a result of mixing.
 * @previous_signal: previously output signal.
 * Return: dc offset signal correlating to the phase shift.
 *
 */
float lowPass_filter(double mixed_signal, double previous_signal)
{
	return ALPHA * mixed_signal + (1 - ALPHA) * previous_signal;
}
