#define alpha 0.5

/**
 * average_shift - determines the average phase shift between 
 * the received signal and the local oscillator.
 * @received_signal: signal received.
 * @local_osc: output of the local oscillator.
 * @previous_signal: previous output signal.
 * Return: the phase shift in degrees.
 */

float average_shift(float mixed_signal, float previous_signal)
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
float lowPass_filter(float mixed_signal, float previous_signal)
{
	return alpha * mixed_signal + (1 - alpha) * previous_signal;
}
