#define ALPHA 0.009

/**
 * average_shift - determines the average phase shift between 
 * the received signal and the local oscillator.
 * @received_signal: signal received.
 * @local_osc: output of the local oscillator.
 * @previous_signal: previous output signal.
 * Return: the phase shift in degrees.
 */

float average_shift(double mixed_signal, double previous_signal)
{
	return lowPass_filter(mixed_signal, previous_signal);
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

