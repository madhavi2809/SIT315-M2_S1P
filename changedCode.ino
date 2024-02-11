#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
      
  Serial.begin(9600);
  
  startTimer(0.5); // Example: Start timer with frequency of 0.5 Hz
}

void loop() { 
  // Nothing to do in the main loop as the blinking is handled by interrupts
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
