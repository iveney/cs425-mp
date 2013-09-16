#!/bin/bash

KEYVALUE="there:morning and ask is this something people will pay for is this of all"
awk -F ':' "/$KEYVALUE/" ../../machine.1.log > 1
awk -F ':' "/$KEYVALUE/" ../../machine.2.log > 2
