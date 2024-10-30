#include "main.h"
#define CARRIER_FREQUENCY 100000
#define PI 3.141592654
#define OMEGA (2 * PI * CARRIER_FREQUENCY)
#define SAMPLING_FREQUENCY 300000
#define SIGNAL_PIN A0


double angle = 0.0;
double signal = 0.0;
double phase_error = 0.0;

double offset_angle_to_oscillator = 0.0;
double oscillator_signal = 0.0;

double I_channel_mixed_signal = 0.0;
double Q_channel_mixed_signal = 0.0;

double I_channel = 0.0;
double I_previous_offset_angle = 0.0;

double Q_channel = 0.0;
double Q_previous_offset_angle = 0.0;

double phase_discriminator = 0.0;
double discriminator_prev = 0.0;



void setup()
{
  pinMode(SIGNAL_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  double step = 0;
  for (step = 0; step < SAMPLING_FREQUENCY; step++)
  {
      angle = OMEGA * step / SAMPLING_FREQUENCY;
      signal = map(analogRead(SIGNAL_PIN));

      phase_error = phase_angle_detector(signal, angle);

      
      oscillator_signal = cos(angle - phase_error);

      Serial.print("Rx: ");
      Serial.println(signal);

      Serial.print("L_osc: ");
      Serial.println(oscillator_signal);

      Serial.print("Phase Angle");
      Serial.println(phase_error);

      Serial.println("");

  }
}



/**
 * phase_angle_detector - gets the phase angle.
 * @oscillator_signal: angle of the oscilator signal
 * Return: phase angle in radians.
 */

double phase_angle_detector(double received_signal, double angle)
{
  
      I_channel_mixed_signal = received_signal * cos(angle);
      Q_channel_mixed_signal = received_signal * sin(angle);

      I_channel = lowPass_filter(I_channel_mixed_signal, I_previous_offset_angle);
      I_previous_offset_angle = I_channel;

      Q_channel = lowPass_filter(Q_channel_mixed_signal,  Q_previous_offset_angle);
      Q_previous_offset_angle = Q_channel;


      //phase discriminator
      phase_discriminator = lowPass_filter(I_channel * Q_channel, discriminator_prev);
      discriminator_prev = phase_discriminator;
      offset_angle_to_oscillator = asin(8 * phase_discriminator) / 2;

      return offset_angle_to_oscillator;
}








