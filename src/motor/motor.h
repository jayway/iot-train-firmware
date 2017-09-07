/*
 * Copyright (C) 2017 Tomas Nilsson (joekickass)
 *
 * Distributed under the MIT License
 */
#ifndef TRAIN_MOTOR_MOTOR_H_
#define TRAIN_MOTOR_MOTOR_H_

#include <Arduino.h>

/*
 * Controls the SparkFun Motor Driver (Dual TB6612FNG (1A)), a DC motor driver.
 *
 * Pin breakout:
 *
 *   ------------
 * -| PWMA    VM |-
 * -| AIN2   VCC |-
 * -| AIN1   GND |-
 * -| STBY   A01 |-
 * -| BIN1   A02 |-
 * -| BIN2   B02 |-
 * -| PWMB   B01 |-
 * -| GND    GND |-
 *   ------------
 *
 * Control states:
 *
 *  ---------------------------------------------
 * | IN1 | IN2 | PWM | OUT1 | OUT2 |    Mode     |
 *  ---------------------------------------------
 * |  H  |  H  | H/L |  L   |  L   | Short brake |
 * |  L  |  H  |  H  |  L   |  H   | CCW         |
 * |  L  |  H  |  L  |  L   |  L   | Short brake |
 * |  H  |  L  |  H  |  H   |  L   | CW          |
 * |  H  |  L  |  L  |  L   |  L   | Short brake |
 * |  L  |  L  |  H  |  OFF |  OFF | Stop        |
 *  ---------------------------------------------
 *
 */
static const uint8_t STBY   = D1;
static const uint8_t AIN1   = D2;
static const uint8_t AIN2   = D3;
static const uint8_t PWMA   = D4;

class Motor
{
public:
  Motor();
  ~Motor();

  void init();
  void forward(uint16_t speed);
  void backward(uint16_t speed);
  void stop();

private:

  bool initalized;
};

#endif  // TRAIN_MOTOR_MOTOR_H_
