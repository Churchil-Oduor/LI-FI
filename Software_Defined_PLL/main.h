#ifndef MAIN_H
#define MAIN_H

float average_shift(double received_signal, double local_osc, double previous_signal);
float lowPass_filter(double mixed_signal, double previous_signal);

#endif
