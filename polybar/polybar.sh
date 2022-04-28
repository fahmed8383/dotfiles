#!/usr/bin/env sh

# Terminate already running bar instances and script
killall -q polybar
killall -q /home/box/.config/polybar/polybar-autohide

# Wait until the processes have been shut down
while pgrep -u $UID -x polybar >/dev/null; do sleep 1; done

# Launch polybar
polybar main-bspwm&
polybar secondary-bspwm&
polybar main-apps&
polybar main-info&
polybar main-power&

# Wait till all 5 bars have been started before start autohide script
while [ $(pgrep -c -x polybar) -ne 5 ]; do sleep 1; done
sleep 1

/home/box/.config/polybar/polybar-autohide &
