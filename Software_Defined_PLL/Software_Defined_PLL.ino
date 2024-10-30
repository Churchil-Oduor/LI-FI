#include "main.h"
#define CARRIER_FREQUENCY 100000
#define PI 3.141592654
#define OMEGA 2 * PI * CARRIER_FREQUENCY
#define SAMPLING_FREQUENCY 300000

double offset_angle_to_oscillator = 0.9;
double oscillator_signal = 0.0;

double I_channel_mixed_signal = 0.0;
double Q_channel_mixed_signal = 0.0;

double I_offset_angle = 0.0;
double I_previous_offset_angle = 0.0;

double Q_offset_angle = 0.0;
double Q_previous_offset_angle = 0.0;

double phase_discriminator = 0.0;
double discriminator_prev = 0.0;

double signal = 0.0;
double angle = 0.0;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  double step = 0;
  for (step = 0; step < SAMPLING_FREQUENCY; step++)
  {
      angle = OMEGA * step / SAMPLING_FREQUENCY;
      signal = cos(angle);

      I_channel_mixed_signal = signal * cos(angle +  offset_angle_to_oscillator);
      Q_channel_mixed_signal = signal * sin(angle +  offset_angle_to_oscillator);

      I_offset_angle = lowPass_filter(I_channel_mixed_signal, I_previous_offset_angle);
      I_previous_offset_angle = I_offset_angle;
     
      Q_offset_angle = lowPass_filter(Q_channel_mixed_signal,  Q_previous_offset_angle);
      Q_previous_offset_angle = Q_offset_angle; 


      //phase discriminator
      phase_discriminator = lowPass_filter(I_offset_angle * Q_offset_angle, discriminator_prev);
      discriminator_prev = phase_discriminator;

      offset_angle_to_oscillator = phase_discriminator * -1; 


     

      Serial.print("Q: ");
      Serial.println(Q_offset_angle);

      Serial.print("I: ");
      Serial.println(I_offset_angle);
      Serial.println(offset_angle_to_oscillator, 6);


    
     
  }
}

























