#!/bin/bash

VALUE="the unique value in"
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.1.log > 1
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.2.log > 2
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.3.log > 3
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.4.log > 4
