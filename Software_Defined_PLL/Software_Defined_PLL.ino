#include "main.h"
#define CARRIER_FREQUENCY 103822
#define PI 3.141592654
#define OMEGA (2 * PI * CARRIER_FREQUENCY)
#define SAMPLING_FREQUENCY 500000
#define SIGNAL_PIN A0


double angle = 0.0;
double signal_Value = 0.0;
double phase_error = 0.0;

double offset_angle_to_oscillator = 0.0;
double error = 0.0;

double I_channel_mixed_signal = 0.0;
double Q_channel_mixed_signal = 0.0;

double I_channel = 0.0;
double I_previous_offset_angle = 0.0;

double Q_channel = 0.0;
double Q_previous_offset_angle = 0.0;

double phase_discriminator = 0.0;
double discriminator_prev = 0.0;
double step = 0;
double LOC_OSC_signal = 0.0;





void setup()
{
  pinMode(SIGNAL_PIN, INPUT);
  Serial.begin(9600);
}
  
void loop()
{
  angle = OMEGA * step/SAMPLING_FREQUENCY ;
  signal_Value = analogRead(SIGNAL_PIN);
  double mapped = 0.787 + mapFloat(signal_Value, 0, 1023, -100, 100) / 100 ;


 // signal_Value = cos(angle + 0.5);
  phase_error = phase_angle_detector(mapped, angle, error);
  error = phase_error;
 


 Serial.println(mapped);


  


  step++;
  delayMicroseconds(2);

  
}



/**
 * phase_angle_detector - gets the phase angle.
 * @oscillator_signal: angle of the oscilator signal
 * Return: phase angle in radians.
 */

double phase_angle_detector(double received_signal, double angle, double error)
{
  
      I_channel_mixed_signal = received_signal * cos(acos(cos(angle)));
      Q_channel_mixed_signal = received_signal * sin(asin(sin(angle)));
    //  Serial.println(acos(cos(angle)) + error);

       

      I_channel = lowPass_filter(I_channel_mixed_signal, I_previous_offset_angle);
      I_previous_offset_angle = I_channel;
        

      Q_channel = lowPass_filter(Q_channel_mixed_signal,  Q_previous_offset_angle);
      Q_previous_offset_angle = Q_channel;

      //phase discriminator
      //phase_discriminator = lowPass_filter(I_channel * Q_channel, discriminator_prev);
      //discriminator_prev = phase_discriminator;
     // offset_angle_to_oscillator = asin(8 * phase_discriminator) / 2;
      offset_angle_to_oscillator = atan(Q_channel/I_channel);

      return offset_angle_to_oscillator;
}

double mapFloat(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
