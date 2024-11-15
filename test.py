import numpy as np
import matplotlib.pyplot as plt

# Constants
Fs = 1e6         # Sampling frequency (1 MHz)
T = 1 / Fs       # Sample period
f_c = 100e3      # Carrier frequency (100 kHz)
N = 1000         # Number of samples
Kp = 0.1         # Phase loop gain
Kd = 0.1         # Frequency loop gain

# Generate a test signal (QAM) - modulated signal
t = np.arange(0, N) * T
# Example: 4QAM modulation (I and Q components)
I = np.cos(2 * np.pi * f_c * t)   # In-phase component
Q = np.sin(2 * np.pi * f_c * t)    # Quadrature component
modulated_signal = I + 1j * Q

# Initialize Costas Loop variables
phase_estimate = 0.0  # Initial phase estimate
freq_estimate = 0.0   # Initial frequency estimate
demodulated_signal = np.zeros(N, dtype=complex)
error_signal = np.zeros(N)

# Costas loop implementation
for n in range(1, N):
    # Multiply by the complex conjugate of the phase estimate
    demodulated_signal[n] = modulated_signal[n] * np.exp(-1j * phase_estimate)
    
    # Extract the I and Q components
    I_demodulated = demodulated_signal[n].real
    Q_demodulated = demodulated_signal[n].imag

    # Calculate the error signal
    error_signal[n] = Kp * (I_demodulated * Q_demodulated)

    # Update the phase and frequency estimates
    phase_estimate += Kd * error_signal[n]
    
    # Wrap the phase estimate to avoid overflow
    phase_estimate = np.arctan2(np.sin(phase_estimate), np.cos(phase_estimate))

# Plot results
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(t, np.real(demodulated_signal), label='Demodulated I component')
plt.plot(t, np.imag(demodulated_signal), label='Demodulated Q component')
plt.title('Demodulated Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend()
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(t, error_signal, label='Error Signal')
plt.title('Costas Loop Error Signal')
plt.xlabel('Time (s)')
plt.ylabel('Error')
plt.legend()
plt.grid()

plt.tight_layout()
plt.show()

