#!/bin/bash

platformio run
cp .pioenvs/megaatmega2560/firmware.hex firmware.firm
sshpass -f ~/n2pass scp firmware.firm 10.1.1.31:
