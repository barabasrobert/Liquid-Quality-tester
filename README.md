# Liquid quality meter

Simple DIY liquid color sensor using an Arduino, RGB LED, and a photoresistor.  
Detects wine/liquid types (dark red, bright red, rosé, yellowish, colorless) by measuring light transmission under different LED colors.

## Hardware

- Arduino Uno
- RGB LED (KY-016)
- Photoresistor (KY-018)
- test tube holder (plans included)

**Pinout:**

- Red channel → A3 (PWM)
- Green channel → A1 (PWM)
- Blue channel → A2 (PWM)
- Sensor → A0

## How it works

1. Cycles through RED, YELLOW, GREEN, WHITE LEDs
2. Measures transmitted light intensity with photoresistor
3. Calculates ratios (red/white, yellow/white)
4. Classifies liquid using a simple decision tree

## Current detection (based on calibration data)

- Dark red
- Bright red
- Rosé
- Yellow
- Water
- Nothing
