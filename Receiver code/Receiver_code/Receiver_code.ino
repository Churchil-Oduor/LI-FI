#define CARRIER_FREQ 100000  // Carrier frequency in Hz (100 kHz)
#define SAMPLE_RATE 1000000  // Sample rate in Hz (1 MHz)
#define PI 3.141592653589793
#define RECEIVED_SIGNAL_PIN A0

// Constants for loop filter and VCO
float vcoFrequency = CARRIER_FREQ;  // VCO frequency (starts at carrier frequency)
float phaseError = 0.0;
float previousPhaseError = 0.0;
float alpha = 0.009;  // Loop filter constant for stability and convergence
float integrator = 0.0;  // Integrator for phase correction

// Low-pass filter constants
float lpAlpha = 0.1;  // Low-pass filter constant for smoothing
float previousI = 0.0;
float previousQ = 0.0;

// Function to generate a local oscillator (VCO)
float generateVCO(float time) {
    return 2 * PI* vcoFrequency * time / SAMPLE_RATE;
}

// Function to apply low-pass filter to the signal
float lowPassFilter(float signal, float alpha, float previousOutput) {
    return alpha * signal + (1 - alpha) * previousOutput;
}

// Phase detector function to compute the phase error
float phaseDetector(float I, float Q) {
    return atan2(Q, I);  // Compute the phase error from the I and Q signals
}

// Function to update the VCO frequency using a loop filter
float loopFilter(float phaseError) {
    integrator += phaseError * alpha;  // Integrate the phase error over time
    return integrator;  // Return the correction term for VCO frequency
}

// Function to extract the I (in-phase) signal
float extractISignal(float receivedSignal, float vcoPhase) {
    return receivedSignal * cos(vcoPhase);
}

// Function to extract the Q (quadrature) signal
float extractQSignal(float receivedSignal, float time, float vcoPhase) {
    return receivedSignal * sin(vcoPhase);
}

// Map I and Q to bit pairs for 4QAM
void mapIQtoBits(float I, float Q) {
    const float Ithreshold = 0;
    const float Qthreshold = 0;
    if (I >= Ithreshold && Q > Qthreshold) {
        Serial.println("Bit: 11");
    } else if (I > Ithreshold && Q < Qthreshold) {
        Serial.println("Bit: 10");
    } else if (I < Ithreshold && Q > Qthreshold) {
        Serial.println("Bit: 01");
    } else if (I < Ithreshold && Q < Qthreshold) {
        Serial.println("Bit: 00");
    }
}


 // Set the ADC prescaler to a lower value for higher sample rate
void setup() {
    

    pinMode(RECEIVED_SIGNAL_PIN, INPUT);
    ADCSRA &= ~(1 << ADPS2);  
    ADCSRA |= (1 << ADPS1);   
    ADCSRA |= (1 << ADPS0); 
    Serial.begin(9600);
}



void loop() {
    for (int time = 0; time < SAMPLE_RATE; time++) {
        float receivedSignal = analogRead(RECEIVED_SIGNAL_PIN);
        //receivedSignal = (receivedSignal / 512.0) - 1.0;

        float vcoPhase = generateVCO(time);

        float I = extractISignal(receivedSignal, vcoPhase);
        float Q = extractQSignal(receivedSignal, vcoPhase);

        // Print raw I and Q values for debugging
        float elapsedTime = time / (float)SAMPLE_RATE; // Convert time to seconds
       
        I = lowPassFilter(I, lpAlpha, previousI);
        Q = lowPassFilter(Q, lpAlpha, previousQ);
        previousI = I;
        previousQ = Q;

        phaseError = phaseDetector(I, Q);

        float correction = loopFilter(phaseError);
        vcoFrequency += correction;

        // Ensure the VCO frequency stays stable
        vcoFrequency = constrain(vcoFrequency, CARRIER_FREQ - 1000, CARRIER_FREQ + 1000);

        // Map I and Q to bit pairs
        mapIQtoBits(I, Q);

        delayMicroseconds(10); // Adjust this delay if necessary
    }
}
