#!/bin/bash

VALUE="by quoting him but i have to risk it because his email was such a"
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.1.log > 1
awk -F ':' "\$2 ~ /$VALUE/" ../../machine.2.log > 2
