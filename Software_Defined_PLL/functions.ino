#define ALPHA 0.005

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

