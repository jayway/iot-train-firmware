/*
 * Copyright (C) 2017 Tomas Nilsson (joekickass)
 *
 * Distributed under the MIT License
 */
#include "motor.h"

Motor::Motor() :
  initalized(false)
{
}

Motor::~Motor()
{
}

void Motor::init()
{
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);
  initalized = true;
}

void Motor::forward(uint16_t speed)
{
  if (initalized) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, speed);
    digitalWrite(STBY, HIGH);
  }
}

void Motor::backward(uint16_t speed)
{
  if (initalized) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, speed);
    digitalWrite(STBY, HIGH);
  }
}

void Motor::stop()
{
  if (initalized) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 0);
    digitalWrite(STBY, HIGH);
  }
}
