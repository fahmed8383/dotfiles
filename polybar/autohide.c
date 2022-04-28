#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xdo.h>

#define HIDE_MARGIN 60
#define MONITORS_NUM 2
#define SLEEP_DELAY 500000

struct monitor {
    char *name;
    int width;
};

// Monitors in order from left to right
const struct monitor monitors[MONITORS_NUM] = {{"HDMI-0", 1920}, {"DP-2", 2560}};

// Array to count the number of windows for each monitor
int window_monitor_count[MONITORS_NUM];

// Function to find and return a 2D array mapping each taskbar to the specified monitor index
Window** find_bars(xdo_t *xdo) {

    // Initialize xdo_search_t struct to seach for "Bar" window class
    xdo_search_t search_query = {0, "Bar", 0, 0, 0, 0, -1, 0, 0, SEARCH_ANY, 0|SEARCH_CLASS, 0, 0};

    // List of found taskbars and the size of the list
    Window *taskbars;
    unsigned int n_win = 0;

    // Search for taskbar windows
    xdo_search_windows(xdo, &search_query, &taskbars, &n_win);
    if(n_win == 0){
        perror("Unable to find any bars");
        exit(EXIT_FAILURE);
    }

    // Array to map each window index to a monitor index
    int window_to_monitor[n_win];

    // Loop through all taskbar windows
    for(int i = 0; i < n_win; i++){

        // Get name of all windows
        unsigned char *name;
        int name_len;
        int name_type;
        xdo_get_window_name(xdo, taskbars[i], &name, &name_len, &name_type);

        // Get the monitor name from window name
        char *display_name_cut = strchr(name, '_');
        display_name_cut++;

        // If names match then map that monitor to the corresponding window
        for(int j = 0; j < MONITORS_NUM; j++){
            if(strcmp(display_name_cut, monitors[j].name) == 0){
                window_to_monitor[i] = j;
                window_monitor_count[j]++;
            }
        }
    }

    // Allocate memory for output windows 2d array
    Window **windows_array = malloc(MONITORS_NUM*sizeof(Window*));
    for(int i = 0; i < MONITORS_NUM; i++){
        windows_array[i] = malloc(window_monitor_count[i]*sizeof(Window));
    }

    // Assign each task bar to the appropriate monitor index
    int monitor_curr_ind[MONITORS_NUM] = {0};
    for(int i = 0; i < n_win; i++){
        int monitor_num = window_to_monitor[i];
        windows_array[monitor_num][monitor_curr_ind[monitor_num]] = taskbars[i];
        monitor_curr_ind[monitor_num]++;
    }

    return windows_array;
}

// Loop through all bars in a corresponding monitor and hide them
void hide_bars(xdo_t *xdo, int monitor_index, Window **taskbars){
    Window *curr_bar = taskbars[monitor_index];
    for(int i = 0; i < window_monitor_count[monitor_index]; i++){
        xdo_unmap_window(xdo, *curr_bar);
        curr_bar++;
    }
}

// Loop through all bars in a corresponding monitor and display them
void show_bars(xdo_t *xdo, int monitor_index, Window **taskbars){
    Window *curr_bar = taskbars[monitor_index];
    for(int i = 0; i < window_monitor_count[monitor_index]; i++){
        xdo_map_window(xdo, *curr_bar);
        curr_bar++;
    }
}

int main() {

    //Create xdo from default display and check for any errors
    xdo_t *xdo = xdo_new(NULL);
    if(xdo == NULL){
        perror("Unable to initialize xdo");
        exit(EXIT_FAILURE);
    }

    // Get all taskbar for each monitor index
    Window **taskbars = find_bars(xdo);

    int hidden = 0;

    // Infinite loop to continousely keep track of mouse position
    while(1){

        // Variables to store the position of the mouse
        int y;
        int x;

        // Get the current mouse position
        xdo_get_mouse_location(xdo, &x, &y, NULL);
        if(&x == NULL || &y == NULL){
            perror("Unable to get mouse position");
            exit(EXIT_FAILURE);
        }

        // Get the current monitor from x value
        int curr_monitor = -1;
        int curr_x_lim = 0;
        for(int i = 0; i < MONITORS_NUM; i++){
            curr_x_lim += monitors[i].width;
            if(x <= curr_x_lim){
                curr_monitor = i;
                break;
            }
        }

        // Make sure we were able to find a monitor from x position
        if(curr_monitor == -1){
            perror("Unable to find corresponding monitor to cursor position. Please ensure that your monitor width's are correct");
            exit(EXIT_FAILURE);
        }

        // If our y value is greater than margin and bar is not hidden yet,
        // hide the bar
        if(!hidden && y > HIDE_MARGIN){
            hide_bars(xdo, curr_monitor, taskbars);
            hidden = 1;
        } 

        // If our y value is less than margin and bar is still hidden,
        // show the bar
        else if(hidden && y <= HIDE_MARGIN){
            show_bars(xdo, curr_monitor, taskbars);
            hidden = 0;
        }

        // Sleep for one second before continuing loop
        usleep(SLEEP_DELAY);
    }

    // Free dynamically allocated memory
    for(int i = 0; i < MONITORS_NUM; i++){
        free(taskbars[i]);
    }
    free(taskbars);

    return 0;
}