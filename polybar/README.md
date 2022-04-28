# Autohide script

# Configuration

Please configure the following parameters in the script to fit your set up:
1. HIDE_MARGIN: Y axis margin after which you wish to hide the bar with the top of the monitor being 0.
2. MONITORS_NUM: The number of monitors you have set up.
3. SLEEP_DELAY: How often you would like the script to check for mouse position and update the bar visibility (with 1 being one second).

## Monitors:

Monitors is a constant array of structures that defines the set up of the monitors, the number of entries in this array must be same as MONITOR_NUM.

The monitors must be added to the array in the correct order of left to right.
**This script only supports monitors next to eachother and not above and beneath eachother.**

The name parameter in monitor struct is the name of the display on your pc, this can be found by using the ``xrandr`` command.
The width parameter in monitor struct is the width of the corresponding monitor.

# Compilation:

The xdotool libarary and xdo header file is required to compile this scrip to a binary. Once you have the required dependencies installed, use the following command to compile this to a binary:

````
gcc autohide.c -lxdo -o polybar-autohide
````

This binary can then be added to your bspwmrc autostart.