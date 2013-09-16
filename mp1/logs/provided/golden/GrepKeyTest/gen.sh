#!/bin/bash


KEY=two
awk -F ':' "\$1 ~ /$KEY/" ../../machine.1.log > 1
awk -F ':' "\$1 ~ /$KEY/" ../../machine.2.log > 2
