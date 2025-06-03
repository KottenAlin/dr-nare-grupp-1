# Motor Calibration System

This module provides a comprehensive motor calibration and testing system for drone development with ESP32 and PS4 controller.

## Features

- **ESC Calibration**: Automatic ESC calibration sequence
- **Individual Motor Testing**: Test motors one by one
- **Speed Testing**: Test at minimum, medium, and progressive speeds
- **Direction Testing**: Verify motor rotation directions
- **Safety Features**: Emergency stop, controller disconnect handling
- **Real-time Status**: System status monitoring

## Hardware Requirements

- ESP32 development board
- 4 ESCs connected to GPIO pins 33, 26, 27, 25
- PS4 controller (paired with ESP32)
- Brushless motors
- Power supply for ESCs/motors

## Motor Layout

```
[0] Front Vertical (CCW)    [1] Front Horizontal (CW)
            \                      /
             \                    /
              \                  /
               \                /
                \              /
                 \            /
                  \          /
                   \        /
                    \      /
                     \    /
                      \  /
                       \/
                       /\
                      /  \
                     /    \
                    /      \
                   /        \
                  /          \
                 /            \
                /              \
               /                \
              /                  \
             /                    \
            /                      \
[2] Back Vertical (CCW)    [3] Back Horizontal (CW)
```

## PS4 Controller Mapping

### Main Controls

- **Triangle**: Enter/Exit Calibration Mode
- **Cross**: Emergency Stop (all motors to minimum)

### Testing Controls (Available in Calibration Mode)

- **Square**: Test all motors at minimum speed
- **Circle**: Test all motors at medium speed
- **L1**: Progressive speed test (gradual increase/decrease)
- **R1**: Motor direction test (individual motor verification)

### Individual Motor Testing

- **D-Pad Up**: Test Front Vertical motor (Motor 0)
- **D-Pad Right**: Test Front Horizontal motor (Motor 1)
- **D-Pad Down**: Test Back Vertical motor (Motor 2)
- **D-Pad Left**: Test Back Horizontal motor (Motor 3)

## Usage

### 1. Include the Header

```cpp
#include <motor_calibration.hpp>
```

### 2. Create Instances

```cpp
Input input;
Servo esc[4];
MotorCalibration motorCalibration(esc, &input);
```

### 3. Initialize in Setup

```cpp
void setup() {
    // ... other setup code ...
    motorCalibration.initialize();
}
```

### 4. Handle in Loop

```cpp
void loop() {
    motorCalibration.safetyCheck();
    motorCalibration.handleCalibrationInput();

    if (motorCalibration.isCalibrationMode()) {
        return; // Skip normal flight operations
    }

    // ... normal flight code ...
}
```

## Safety Features

1. **Controller Disconnect Detection**: Automatically stops all motors if controller disconnects
2. **Emergency Stop**: Immediate motor shutdown with Cross button
3. **Calibration Mode Isolation**: Normal flight operations disabled during calibration
4. **Gradual Speed Changes**: Progressive tests include gradual speed transitions
5. **Button Debouncing**: Prevents accidental multiple activations

## Calibration Sequence

1. **Enter Calibration Mode**: Press Triangle button
2. **ESC Calibration**: Automatic high-to-low throttle sequence
3. **Testing**: Use various test functions to verify motor operation
4. **Exit**: Press Triangle again to return to normal flight mode

## Throttle Values

- **Minimum**: 1000µs (motor stop)
- **Low Speed**: 1100µs
- **Medium Speed**: 1400µs
- **Maximum**: 1700µs
- **Calibration High**: 1900µs (ESC calibration only)

## Safety Recommendations

1. **Remove Propellers**: Always remove propellers during initial testing
2. **Secure Mounting**: Ensure drone/motors are securely mounted
3. **Clear Area**: Test in open area away from people and objects
4. **Emergency Access**: Keep emergency stop (Cross button) easily accessible
5. **Visual Inspection**: Inspect all connections before testing
6. **Power Supply**: Ensure adequate power supply for all motors

## Troubleshooting

### Controller Not Connecting

- Check PS4 controller pairing
- Verify MAC address in input.cpp
- Ensure controller is charged

### Motors Not Responding

- Check ESC connections
- Verify power supply
- Ensure ESC calibration completed
- Check motor wiring

### Inconsistent Motor Behavior

- Re-run ESC calibration sequence
- Check for loose connections
- Verify PWM signal integrity

## Integration with Main Flight Code

The motor calibration system is designed to integrate seamlessly with your main flight control code:

```cpp
// In main loop
motorCalibration.handleCalibrationInput();

if (motorCalibration.isCalibrationMode()) {
    delay(50);
    return; // Skip PID and normal flight operations
}

// Normal PID flight control continues here...
```

This ensures calibration and normal flight operations don't interfere with each other.
