#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pca9685 as pwm
from time import sleep

pwm.init(0,50)

MIN_VAL=100
MAX_VAL=500
step=1
value=MIN_VAL
while True:

	if(value > MAX_VAL):
		step = -1
	if(value < MIN_VAL):
		step = 1
	value = value + step
	pwm.set(value, value)
	sleep(0.01)
