#!/bin/bash

conky -c $(dirname $0)/clock.conf &
conky -c $(dirname $0)/cpu_ram.conf &
conky -c $(dirname $0)/stocks.conf &
conky -c $(dirname $0)/twitch.conf &
