#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
      
  Serial.begin(9600);
  
  startTimer();
}

void loop() { 
  // Nothing to do in the main loop as the blinking is handled by interrupts
}

void startTimer() {
  noInterrupts(); // Disable interrupts
  
  // Set Timer1 in CTC mode with prescaler of 1024
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1 = 0; // Initialize counter value to 0
  
  // Set compare match register for 1Hz increments
  OCR1A = 15624; // = (16e6) / (1024 * 1) - 1 (1Hz) - 1
  
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
