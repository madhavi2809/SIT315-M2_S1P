#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte POTENTIOMETER_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
      
  Serial.begin(9600);
  
  startTimer(0.5); // Initial timer frequency set to 0.5 Hz
}

void loop() { 
  // Read potentiometer value and map it to the desired frequency range
  double potValue = analogRead(POTENTIOMETER_PIN);
  double frequency = mapPotValueToFrequency(potValue);
  
  // Update timer frequency
  startTimer(frequency);
  
  // Other operations can be added here if needed
}

void startTimer(double timerFrequency) {
  noInterrupts(); // Disable interrupts
  
  // Calculate the OCR1A value based on timer frequency
  uint16_t ocrValue = static_cast<uint16_t>((F_CPU / 1024 / timerFrequency) - 1);
  
  // Set Timer1 in CTC mode with prescaler of 1024
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1 = 0; // Initialize counter value to 0
  
  // Set compare match register for the calculated frequency
  OCR1A = ocrValue;
  
  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  
  // Set CS12, CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  
  // Enable Timer1 interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED
}

double mapPotValueToFrequency(double potValue) {
  // Map the analog value of potentiometer (0-1023) to a frequency range (e.g., 0.1 Hz to 10 Hz)
  double frequency = map(potValue, 0, 1023, 0.1, 10); // Adjust this range according to your requirement
  return frequency;
}
