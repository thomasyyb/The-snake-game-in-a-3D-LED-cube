#ifndef _LED_CUBE
#define _LED_CUBE

#include "snake_class.hh"
// ground 48 49 50 51 52 53
// column 0~5 6~11 14~19 22~27 28~33 34~39
// GPIO 0~5        -->  x=5, y=5,4,3,2,1,0
// GPIO 6~9,12,13  -->  x=0, y=5,4,3,2,1,0
// GPIO 14~19      -->  x=4, y=0,1,2,3,4,5
// GPIO 22~27      -->  x=3, y=0,1,2,3,4,5
// GPIO 28~33      -->  x=2, y=0,1,2,3,4,5
// GPIO 34~39      -->  x=1, y=5,4,3,2,1,0

void display_cube(bool* led);

void plane_split(Snake snake, bool* led);

void general_plane_split(cube_point* cube_group, int size, bool* led);

void led_cube_setup();

void led_column_control(int x, int y, bool state);

void led_column_off();

void led_plane_control(int plane, bool state);

void led_column_select(int column, bool state);

void led_plane_select(int plane);

void control_all(bool state);

void column_demo();

void alphabet(char alpha);

void coooooooool();

void superme();

#endif
