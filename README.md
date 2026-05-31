# Smart Room Automation System

## Overview

The Smart Room Automation System is an embedded systems project developed as part of a university team project. The system automatically monitors environmental conditions and controls room functions such as lighting, temperature management, and security monitoring.

By integrating multiple sensors with an Arduino-based controller, the system creates an intelligent room environment capable of operating in different modes, including automatic operation, night-time lighting, and security monitoring.

---

## Features

### Temperature Monitoring & Fan Control
- Continuously monitors room temperature.
- Automatically activates the cooling fan when the temperature exceeds a predefined threshold.
- Turns the fan off when the temperature returns to a safe level.

### Automatic Day/Night Detection
- Uses an LDR (Light Dependent Resistor) to detect ambient light levels.
- Automatically switches between Day Mode and Night Mode.

### Motion Detection
- Uses a PIR motion sensor to detect movement.
- Activates room lighting during Night Mode when motion is detected.
- Keeps the system idle when no motion is present.

### Smart Lighting
- Automatically turns the room light on when motion is detected at night.
- Prevents unnecessary lighting during daytime conditions.

### Security / Away Mode
- Allows the system to operate as a basic security system.
- Detects movement while in Away Mode.
- Triggers an alert when unauthorized motion is detected.

### User Interaction
- Push button controls system functions.
- Long button press clears active security alerts.
- LCD display provides real-time system status and sensor information.

---

## System Architecture

### Inputs
- Temperature Sensor
- Light Sensor (LDR)
- PIR Motion Sensor
- Push Button

### Processing
- Arduino Uno
- Mode Management Logic
- Sensor Monitoring Logic
- Alert Handling Logic

### Outputs
- Cooling Fan
- Room Light (Yellow LED)
- LCD Display
- Security Alert Indicators

---

## Operating Modes

### Day Mode
- Room light remains off.
- Motion detection does not activate lighting.
- Temperature control remains active.

### Night Mode
- Motion detection activates room lighting.
- Lighting automatically turns off after inactivity.
- Temperature control remains active.

### Away Mode
- Motion detection triggers a security alert.
- Alarm remains active until cleared by a long button press.

### Alert State
- Activated when motion is detected during Away Mode.
- Security indicators remain active until reset.

---

## Hardware Components

| Component | Purpose |
|------------|------------|
| Arduino Uno | Main controller |
| Temperature Sensor | Temperature monitoring |
| LDR Sensor | Day/Night detection |
| PIR Motion Sensor | Motion detection |
| RGB LCD Display | Status display |
| Yellow LED | Room lighting indicator |
| Cooling Fan | Temperature control |
| Push Button | User control |

---

## Demonstration

The system demonstrates:

1. Automatic fan activation based on temperature.
2. Automatic switching between Day and Night modes.
3. Motion-activated lighting during Night Mode.
4. Idle behaviour when no motion is detected.
5. Security alert activation in Away Mode.
6. Alert clearing using a long button press.
7. Continuous operation of temperature control across all modes.

---

## Technologies Used

- Arduino
- Embedded C/C++
- Sensor Integration
- Real-Time Embedded Systems
- Hardware Interfacing
- Automation Systems
- System Testing and Validation

---


## Future Improvements

- IoT connectivity and remote monitoring
- Mobile application integration
- Data logging and analytics
- Cloud-based alert notifications
- Wireless sensor support
- Multi-room monitoring

---
