#define F_CPU 16000000UL
#define SOUND_SPEED 0.0343
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define TRIG_PIN PD2
#define ECHO_PIN PD3
#define LED_GREEN1 PD4
#define LED_GREEN2 PD5
#define LED_YELLOW1 PD6
#define LED_YELLOW2 PD7
#define LED_RED1 PB0
#define LED_RED2 PB1
#define GREEN_LED_DDR DDRD
#define YELLOW_LED_DDR DDRD
#define RED_LED_DDR DDRB
#define TRIG_DDR DDRD
#define ECHO_DDR DDRD
#define TRIG_PORT PORTD
volatile uint16_t currentDistance = 0;
volatile uint16_t closestDistance = 0xFFFF;
volatile uint16_t objectCount = 0;
volatile uint8_t objectDetected = 0;
unsigned int measureDistance() {
PORTD |= (1 << TRIG_PIN);
_delay_us(10);
PORTD &= ~(1 << TRIG_PIN);
unsigned int count = 0;
while (!(PIND & (1 << ECHO_PIN)));
TCNT1 = 0;
TCCR1B |= (1 << CS11);
while (PIND & (1 << ECHO_PIN)) {
if (TCNT1 > 30000) {
break;
}
}
TCCR1B = 0;
count = TCNT1 / 2;
return (double)count * SOUND_SPEED / 2;
}
void USART_Init() {
UCSR0A = 0x00;
UBRR0H = 0x00;
UBRR0L = 0x67;
UCSR0C &= ~((1<<UMSEL00)|(1<<UMSEL01));
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}
void USART_Transmit_Char(char c) {
while (!(UCSR0A & (1<<UDRE0)));
UDR0 = c;
}
void USART_Send_Distance_and_Count(uint16_t distance, uint16_t count) {
const char distancePrefix[] = "Yakin Mesafe: ";
for (uint8_t i = 0; distancePrefix[i] != '\0'; i++) {
USART_Transmit_Char(distancePrefix[i]);
}
if (distance == 0) {
USART_Transmit_Char('0');
} else {
char buffer[6];
uint8_t i = 0;
while (distance > 0) {
buffer[i++] = '0' + distance % 10;
distance /= 10;
}
while (i > 0) {
USART_Transmit_Char(buffer[--i]);
}
}
const char countPrefix[] = " cm, Nesne Sayisi: ";
for (uint8_t i = 0; countPrefix[i] != '\0'; i++) {
USART_Transmit_Char(countPrefix[i]);
}
if (count == 0) {
USART_Transmit_Char('0');
} else {
char buffer[6];
uint8_t i = 0;
while (count > 0) {
buffer[i++] = '0' + count % 10;
count /= 10;
}
while (i > 0) {
USART_Transmit_Char(buffer[--i]);
}
}
USART_Transmit_Char('\r');
USART_Transmit_Char('\n');
}
void setup_io() {
DDRD |= (1 << TRIG_PIN) | (1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 <<
LED_YELLOW1) | (1 << LED_YELLOW2);
DDRB |= (1 << LED_RED1) | (1 << LED_RED2);
DDRD &= ~(1 << ECHO_PIN);
PORTD &= ~((1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2));
PORTB &= ~((1 << LED_RED1) | (1 << LED_RED2));
}
void processMeasurement(uint16_t distance) {
if (distance <= 200 && objectDetected == 0) {
objectDetected = 1;
closestDistance = distance;
} else if (distance > 200 && objectDetected == 1) {
objectDetected = 0;
objectCount++;
USART_Send_Distance_and_Count(closestDistance, objectCount);
closestDistance = 0xFFFF;
} else if (distance <= closestDistance) {
closestDistance = distance;
}
}
ISR(TIMER1_COMPA_vect) {
uint16_t distance = measureDistance();
PORTD &= ~((1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2));
PORTB &= ~((1 << LED_RED1) | (1 << LED_RED2));
if (distance > 200) {
} else if (distance > 100) {
PORTD |= (1 << LED_GREEN1);
} else if (distance > 60) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
} else if (distance > 40) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1);
} else if (distance > 20) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2);
} else if (distance > 10) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2);
PORTB |= (1 << LED_RED1);
} else {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2);
PORTB |= (1 << LED_RED1) | (1 << LED_RED2);
}
}
void setLEDsBasedOnDistance(uint16_t distance) {
PORTD &= ~((1 << LED_GREEN1) | (1 << LED_GREEN2) | (1 << LED_YELLOW1) | (1 <<
LED_YELLOW2));
PORTB &= ~((1 << LED_RED1) | (1 << LED_RED2));
if (distance <= 10) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
PORTD |= (1 << LED_YELLOW1) | (1 << LED_YELLOW2);
PORTB |= (1 << LED_RED1) | (1 << LED_RED2);
} else if (distance <= 20) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
PORTD |= (1 << LED_YELLOW1) | (1 << LED_YELLOW2);
PORTB |= (1 << LED_RED1);
} else if (distance <= 40) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
PORTD |= (1 << LED_YELLOW1) | (1 << LED_YELLOW2);
} else if (distance <= 60) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
PORTD |= (1 << LED_YELLOW1);
} else if (distance <= 100) {
PORTD |= (1 << LED_GREEN1) | (1 << LED_GREEN2);
} else if (distance <= 200) {
PORTD |= (1 << LED_GREEN1);
}
}
int main(void) {
setup_io();
USART_Init();
sei();
while (1) {
uint16_t distance = measureDistance();
processMeasurement(distance);
setLEDsBasedOnDistance(distance);
_delay_ms(100);
}
return 0;
}
