# Autohide script

# Configuration

Please configure the following parameters in the script to fit your set up:
1. HIDE_MARGIN: Y axis margin after which you wish to hide the bar (with the top of the monitor being 0).
2. DEAD_ZONE: This is the extra margin added on after which the bar goes away, this helps with moving mouse to locations near the margin. I would recommend a value of atleast the height of your bar.
3. MONITORS_NUM: The number of monitors you have set up.
4. SLEEP_DELAY: How often you would like the script to check for mouse position and update the bar visibility (with 1000000 being one second).
5. TOP_PADDING: This is the padding you want to add to your windows when the bar is being displayed. This will shift all other content down so the bar does not cover it.


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

Finally add the path to the binary in your polybar.sh script.
**Make sure to update the number in the while loop when waiting for the bars to launch to be equal to the number of bars you have**
