#ifndef MAIN_H
#define MAIN_H

float average_shift(float received_signal, float local_osc, float previous_signal);
float lowPass_filter(float mixed_signal, float previous_signal);

#endif
