# Vehicle Parking Assistant

This repository contains the implementation of a **Vehicle Parking Assistant** designed using an **Mbed** microcontroller and various hardware components. The system assists with parking by providing visual, auditory, and LED-based feedback. It supports both **manual** and **autonomous** parking modes.

---

## Features

1. **LIDAR Sensors**:
   - Measures the distance between the vehicle and nearby obstacles.

2. **uLCD Display**:
   - Displays the distance to an obstacle in real-time.
   - Shows warnings (e.g., "STOP!") when the vehicle is too close.

3. **Speaker**:
   - Emits beeping sounds that increase in speed and volume as the vehicle approaches an obstacle.

4. **LEDs**:
   - Indicates braking and reverse operations.
   - Visualizes obstacle proximity with light effects (e.g., rainbow effects for added complexity).

5. **Bluetooth Control**:
   - Allows manual control via the **BlueFruit Connect App** with controls for driving, reversing, and parking.

6. **Autonomous Mode**:
   - Automatically parks the vehicle using LIDAR data and visual/audio feedback.

---

## Components Used

- **Mbed Microcontroller**
- **2 LIDAR Sensors**
- **uLCD Display**
- **Speaker with Class D Amplifier**
- **Bluetooth Module (Adafruit Bluefruit)**
- **WS2812 LEDs**
- **Brushless DC Motors with H-Bridge Driver**
- **Magician Robot Base Kit**

---

## Functional Overview

### Manual Mode
- Controlled via Bluetooth using the **BlueFruit Connect App**.
- Commands include:
  - **Drive Forward**
  - **Reverse**
  - **Left/Right Turns**
  - **Park**

### Autonomous Mode
- Utilizes LIDAR sensors to measure distances.
- Automatically parks the vehicle while ensuring a safe distance from obstacles.
- Provides real-time feedback through the uLCD, LEDs, and speaker.

### Visual and Audio Feedback
- **uLCD**:
  - Displays distance in millimeters.
  - Shows warnings when obstacles are detected too close.
- **Speaker**:
  - Emits beeping sounds proportional to the proximity of obstacles.
- **LEDs**:
  - Lights up during reversing and parking.
  - Displays a rainbow effect for additional visualization.

---

## Hardware Setup

### Connections
1. **LIDAR Sensors**:
   - Connected to `p6, p7` and `p12, p13` for distance measurement.
2. **uLCD Display**:
   - Connected to `p9, p10, p11`.
3. **Speaker**:
   - Connected to `p18`.
4. **WS2812 LEDs**:
   - Connected to `p20`.
5. **Bluetooth Module**:
   - TX and RX pins connected to `p28, p27`.
6. **Motors**:
   - Left Motor: `p21, p22, p23`.
   - Right Motor: `p26, p25, p24`.

---

## Code Structure

### Main Functionalities

1. **Distance Measurement**:
   - `mu` and `mu1` handle LIDAR updates and distance calculations.

2. **Visual Feedback**:
   - `updateDisplay(distance)`: Updates the uLCD with the current distance.
   - `alertDisplay(distance)`: Displays a "STOP!" warning when too close.

3. **Audio Feedback**:
   - `beep()`: Plays a beeping sound proportional to the distance.

4. **LED Control**:
   - `light_on()`, `light_off()`: Controls LED brightness.
   - `rainbow()`: Displays a rainbow effect.

5. **Vehicle Movement**:
   - `left.speed(value)`, `right.speed(value)`: Controls motor speeds for driving, reversing, and turning.

6. **Bluetooth Control**:
   - Processes commands from the **BlueFruit Connect App** to control the vehicle.

---

## Example Commands

- **Button 1**: Reverse park autonomously.
- **Button 2**: Execute parking maneuver.
- **Button 3**: Autonomous parking.
- **Button 4**: Rainbow LED effect.
- **Button 5**: Drive forward.
- **Button 6**: Reverse manually.
- **Button 7**: Turn left.
- **Button 8**: Turn right.

---

## Setup Instructions

1. **Connect Components**:
   - Wire all components according to the hardware setup.

2. **Upload Code**:
   - Flash the provided code onto the Mbed microcontroller.

3. **Install Bluetooth App**:
   - Download the **BlueFruit Connect App** on your smartphone.

4. **Test Functionality**:
   - Pair the Bluetooth module with the app.
   - Test manual controls and autonomous parking.

---

## Testing

### Manual Mode
- Use the Bluetooth app to control the vehicle.
- Test driving, reversing, and parking.

### Autonomous Mode
- Place the vehicle near an obstacle.
- Activate autonomous mode and observe the parking process.

### Feedback
- Verify the accuracy of distance measurements on the uLCD.
- Ensure LEDs and speaker respond correctly to proximity changes.
