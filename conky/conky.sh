#!/bin/bash

# Terminate already running conky instances
killall -q conky

# Wait until the processes have been shut down
while pgrep -u $UID -x conky >/dev/null; do sleep 1; done

# Launch conky
conky -c $(dirname $0)/clock.conf &
conky -c $(dirname $0)/cpu_ram.conf &
conky -c $(dirname $0)/stocks.conf &
conky -c $(dirname $0)/twitch.conf &
