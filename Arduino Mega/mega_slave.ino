///////////////////////////////////
// Arduino Mega -- slave control //
/////////////////////////////////////////////////////////
// Three-state FSM:
// 1.Before start
//   some demo LED display, wait for start signal
// 2.Game playing
//   control every light, wait for direction change
//   if eat apple --> length++
//                --> tell rpi
//   if hit wall  --> game over
//                --> tell rpi
// 3.Game over
//   same as before start
//////////////////////////////////////////////////////////

#include "snake_class.hh"
#include "led_cube.hh"
#include <SoftwareSerial.h>
#include "stdio.h"

void running_in_circle();
void playing_snake_easy();
void playing_snake_medium();
void coooooooool();

SoftwareSerial portOne(10, 11);         // software serial #1: RX = digital pin 10, TX = digital pin 11

cube_point snake_head = {0,1,0};
cube_point snake_body[216]={{0,1,0},{0,0,0},{0,0,0}};
Snake snake(snake_head, snake_body, 0);
bool led_cube_display[216];
char received_byte = 0;
char working_state = 0;
int direction_test = 0;

void setup() {
  led_cube_setup();
  Serial.begin(9600);                   // Open serial communications and wait for port to open:
  while (!Serial) {
    ;                                   // wait for serial port to connect. Needed for native USB port only
  }
  portOne.begin(9600);                  // Start software serial port
  portOne.listen();
  snake.apple = {3,5,0};
}

unsigned long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)
//char received_data[20];
//char serial_read_data[20];
void loop() {
  // Start Menu
  if(working_state == 0){
    if (portOne.available() > 0){
      char inByte = portOne.read();
      if (inByte == 'S') working_state = 1; // Start Game
      if (inByte == 'D') working_state = 3; // Demo
    }
  }

  // Playing Game
  if(working_state == 1){
    //running_in_circle();
    //playing_snake_easy();
    playing_snake_medium();
    //coooooooool();
    //superme();
    //column_demo();
  }

  // Game Over
  if(working_state == 2){
    alphabet('G');
    alphabet('A');
    alphabet('M');
    alphabet('E');
    alphabet('O');
    alphabet('V');
    alphabet('E');
    alphabet('R');
    if (portOne.available() > 0) {
      char inByte = portOne.read();
      if (inByte == 'R'){
        // Back to main menu
        working_state = 0;
        // Initial the snake
        snake.head = {0,1,0};
        for(int i=0; i<216; i++){
          snake.body[i].x = 0;
          snake.body[i].y = 0;
          snake.body[i].z = 0;
        }
        snake.length = 0;
        snake.snake_direction = Leftward;
        interval = 1000;
      }
    }
  }

  // Demo
  if(working_state == 3){
    coooooooool();
  }
}

void playing_snake_easy(){
  plane_split(snake, led_cube_display);
  display_cube(led_cube_display);

  if (portOne.available() > 0){
    char inByte = portOne.read();
    switch(inByte){
      case '0': snake.direction_change(Foreward); break;
      case '1': snake.direction_change(Leftward); break;
      case '2': snake.direction_change(Upward); break;
      case '3': snake.direction_change(Backward); break;
      case '4': snake.direction_change(Rightward); break;
      case '5': snake.direction_change(Downward); break;
    }
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    snake.snake_move();
    Serial.println("Apple:");
    Serial.println(snake.apple.x);
    Serial.println(snake.apple.y);
    Serial.println(snake.apple.z);
    if(int collide = snake.collide_detection()){
      if(collide == 1){           // eat apple
        portOne.write('+');
      }else if(collide == 2){     // hit body/wall
        portOne.write('!');
        working_state = 2;
      }
    }
    previousMillis = currentMillis;
  }
}

void playing_snake_medium(){
  plane_split(snake, led_cube_display);
  display_cube(led_cube_display);

  if (portOne.available() > 0){
    char inByte = portOne.read();
    switch(inByte){
      case '0': snake.direction_change(Foreward); break;
      case '1': snake.direction_change(Leftward); break;
      case '2': snake.direction_change(Upward); break;
      case '3': snake.direction_change(Backward); break;
      case '4': snake.direction_change(Rightward); break;
      case '5': snake.direction_change(Downward); break;
    }
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    snake.snake_move();
    Serial.println("Apple:");
    Serial.println(snake.apple.x);
    Serial.println(snake.apple.y);
    Serial.println(snake.apple.z);
    if(int collide = snake.collide_detection()){
      if(collide == 1){           // eat apple
        if(interval > 400)
          interval -= 80;
        portOne.write('+');
      }else if(collide == 2){     // hit body/wall
        portOne.write('!');
        working_state = 2;
      }
    }
    previousMillis = currentMillis;
  }
}

void running_in_circle(){
  plane_split(snake, led_cube_display);
  display_cube(led_cube_display);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    if(snake.head.x==0 && snake.head.y==5 && snake.head.z==0)
      snake.direction_change(Foreward);
    if(snake.head.x==5 && snake.head.y==5 && snake.head.z==0)
      snake.direction_change(Rightward);
    if(snake.head.x==5 && snake.head.y==0 && snake.head.z==0)
      snake.direction_change(Upward);
    if(snake.head.x==5 && snake.head.y==0 && snake.head.z==5)
      snake.direction_change(Backward);
    if(snake.head.x==0 && snake.head.y==0 && snake.head.z==5)
      snake.direction_change(Downward);
    if(snake.head.x==0 && snake.head.y==0 && snake.head.z==0)
      snake.direction_change(Leftward);
      
    snake.snake_move();
    Serial.println("Apple:");
    Serial.println(snake.apple.x);
    Serial.println(snake.apple.y);
    Serial.println(snake.apple.z);
    
    if(int collide = snake.collide_detection()){
      if(collide == 1){           // eat apple
        portOne.write('+');
      }else if(collide == 2){     // hit body/wall
        portOne.write('!');
        working_state = 2;
      }
    }
    previousMillis = currentMillis;
  }
}

void coooooooool(){
  bool led_cube_display[216];
  unsigned long previousMillis = 0;
  int interval = 150;
  bool up_or_down[6]={1,0,0,0,0,0};      // Group 1 should go up, others go down
  cube_point group_0[6] ={{0,0,0},{1,1,0},{2,2,0},{3,3,0},{4,4,0},{5,5,0}};
  cube_point group_1[10]={{0,1,1},{1,2,1},{2,3,1},{3,4,1},{4,5,1},{1,0,1},{2,1,1},{3,2,1},{4,3,1},{5,4,1}};
  cube_point group_2[8]={{0,2,2},{1,3,2},{2,4,2},{3,5,2},{2,0,2},{3,1,2},{4,2,2},{5,3,2}};
  cube_point group_3[6]={{0,3,3},{1,4,3},{2,5,3},{3,0,3},{4,1,3},{5,2,3}};
  cube_point group_4[4]={{0,4,4},{1,5,4},{4,0,4},{5,1,4}};
  cube_point group_5[2]={{0,5,5},{5,0,5}};
  
  while(1){
    for (int i=0; i<216; i++)
      led_cube_display[i]=LOW;
    general_plane_split(group_0, 6,  led_cube_display);
    general_plane_split(group_1, 10, led_cube_display);
    general_plane_split(group_2, 8,  led_cube_display);
    general_plane_split(group_3, 6,  led_cube_display);
    general_plane_split(group_4, 4,  led_cube_display);
    general_plane_split(group_5, 2,  led_cube_display);
    display_cube(led_cube_display);

    if (portOne.available() > 0){
      char inByte = portOne.read();
      if (inByte == 'R') working_state = 0; // Start Menu
      control_all(LOW);
      break;
    }
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      for(int i=0; i<6; i++){
        if(up_or_down[0]==1)
          group_0[i].z++;
        else if(up_or_down[0]==0)
          group_0[i].z--;
      }
      if(group_0[0].z==5)
        up_or_down[0]=0;
      if(group_0[0].z==0)
        up_or_down[0]=1;

      for(int i=0; i<10; i++){
        if(up_or_down[1]==1)
          group_1[i].z++;
        else if(up_or_down[1]==0)
          group_1[i].z--;
      }
      if(group_1[0].z==5)
        up_or_down[1]=0;
      if(group_1[0].z==0)
        up_or_down[1]=1;

      for(int i=0; i<8; i++){
        if(up_or_down[2]==1)
          group_2[i].z++;
        else if(up_or_down[2]==0)
          group_2[i].z--;
      }
      if(group_2[0].z==5)
        up_or_down[2]=0;
      if(group_2[0].z==0)
        up_or_down[2]=1;

      for(int i=0; i<6; i++){
        if(up_or_down[3]==1)
          group_3[i].z++;
        else if(up_or_down[3]==0)
          group_3[i].z--;
      }
      if(group_3[0].z==5)
        up_or_down[3]=0;
      if(group_3[0].z==0)
        up_or_down[3]=1;

      for(int i=0; i<4; i++){
        if(up_or_down[4]==1)
          group_4[i].z++;
        else if(up_or_down[4]==0)
          group_4[i].z--;
      }
      if(group_4[0].z==5)
        up_or_down[4]=0;
      if(group_4[0].z==0)
        up_or_down[4]=1;

      for(int i=0; i<2; i++){
        if(up_or_down[5]==1)
          group_5[i].z++;
        else if(up_or_down[5]==0)
          group_5[i].z--;
      }
      if(group_5[0].z==5)
        up_or_down[5]=0;
      if(group_5[0].z==0)
        up_or_down[5]=1;
      previousMillis = currentMillis;
    }
  }
}
