# Ultrasonic Closest Distance and Object Counter

An AVR-based embedded system that uses an **HC-SR04 ultrasonic sensor** to measure distances,  
record the closest distance of passing objects, count them, and provide both **LED feedback** and  
**serial communication output** via USART.

---

## 🎯 Project Purpose
- Measure distance using ultrasonic sound waves.
- Light up LEDs based on the measured distance (green, yellow, red).
- Record the **closest distance** for each passing object.
- Count the total number of detected objects.
- Send both closest distance and object count to a serial monitor via USART.

---

## 🛠 Hardware Used
- **Microcontroller:** ATmega (16 MHz clock)
- **Sensor:** HC-SR04 Ultrasonic Distance Sensor
- **LEDs:** 2 Green, 2 Yellow, 2 Red
- **Connections:**
  - Green LEDs: PD4, PD5
  - Yellow LEDs: PD6, PD7
  - Red LEDs: PB0, PB1
  - TRIG pin: PD2
  - ECHO pin: PD3

---

## 💻 Software & Libraries
- **Language:** C (AVR C)
- **Libraries:** `<avr/io.h>`, `<avr/interrupt.h>`, `<util/delay.h>`
- **Development Environment:** Atmel Studio / AVR-GCC

---

## 🔍 How It Works
1. **Distance Measurement**  
   TRIG pin sends a 10µs pulse to trigger the HC-SR04.  
   ECHO pin returns the time taken for the signal to bounce back.  
   Using the **speed of sound**, the distance is calculated.

2. **LED Indication**  
   - >200 cm: No LED lit  
   - 100–200 cm: 1 Green LED  
   - 60–100 cm: 2 Green LEDs  
   - 40–60 cm: 1 Yellow + 2 Green LEDs  
   - 20–40 cm: 2 Yellow + 2 Green LEDs  
   - 10–20 cm: 1 Red + 2 Yellow + 2 Green LEDs  
   - <10 cm: 2 Red + 2 Yellow + 2 Green LEDs  

3. **Object Counting**  
   - When an object enters and leaves the detection zone,  
     the system increments the count and logs the **closest distance**.

4. **Serial Output**  
   - Closest distance and object count are sent to a serial monitor via USART.

---

## 📜 Files
- `main.c` — Contains all logic for distance measurement, LED control, object counting, and USART communication.
- `ele362_proje_211201074.pdf` — **Full project report (in Turkish)**.

---


✍ **Author:** Erkin Coşkun Ruhi  
📅 **Date:** 08.04.2024  
🏫 **TOBB University of Economics and Technology**
