#include "led_cube.hh"
#include <Arduino.h>

// ground 48 49 50 51 52 53
// column 0~5 6~11 14~19 22~27 28~33 34~39
// GPIO 15,14,2~5  -->  x=5, y=5,4,3,2,1,0
// GPIO 6~9,12,40  -->  x=0, y=5,4,3,2,1,0
// GPIO 16~21      -->  x=4, y=0,1,2,3,4,5
// GPIO 22~27      -->  x=3, y=0,1,2,3,4,5
// GPIO 28~33      -->  x=2, y=0,1,2,3,4,5
// GPIO 34~39      -->  x=1, y=5,4,3,2,1,0

void display_cube (bool* led){
  for (int i=0; i<6; i++){
    led_column_off();
    led_plane_select(i);                          //i represent z
    for(int j=0; j<6; j++)                        //j represents y
      for(int k=0; k<6; k++)                     //k represents x      
        led_column_control(k, j, led[k+j*6+i*36]);
  }
}

void plane_split(Snake snake, bool* led){
 // set all bits to low
 for (int i=0; i<216; i++)
   led[i]=LOW;
   
 // set head
 led[snake.head.x + snake.head.y*6 + snake.head.z*36] = HIGH;
 
//  set body
 for(int i=(snake.length); i>=0; --i){ 
   led[snake.body[i].x + (snake.body[i].y)*6 + (snake.body[i].z)*36] = HIGH;
 }
 
 // set apple
 led[snake.apple.x + snake.apple.y*6 + snake.apple.z*36] = HIGH;
}

void general_plane_split(cube_point* cube_group, int size, bool* led){
 for(int i=size; i>=0; --i){ 
   led[cube_group[i].x + (cube_group[i].y)*6 + (cube_group[i].z)*36] = HIGH;
 }
}


void led_cube_setup(){
  for(int i=2; i<10; i++){
    pinMode(i, OUTPUT);
  }
  for(int i=12;i<13; i++){
    pinMode(i, OUTPUT);
  }
  for(int i=14;i<41; i++){
    pinMode(i, OUTPUT);
  }
  for(int i=48; i<54; i++){
    pinMode(i, OUTPUT);
  }
  for(int i=0; i<6; i++){
    for(int j=0; j<6; j++){
      led_column_control(i, j, LOW);
    }
    led_plane_control(i,LOW);
  }
}

void led_column_control(int x, int y, bool state){
  switch(x){
    case 0:{
      switch(y){
        case 0: digitalWrite(40, state); break;
        case 1: digitalWrite(12, state); break;
        case 2: digitalWrite(9, state); break;
        case 3: digitalWrite(8, state); break;
        case 4: digitalWrite(7, state); break;
        case 5: digitalWrite(6, state); break;
      }break;
    }
    case 1:{
      switch(y){
        case 0: digitalWrite(39, state); break;
        case 1: digitalWrite(38, state); break;
        case 2: digitalWrite(37, state); break;
        case 3: digitalWrite(36, state); break;
        case 4: digitalWrite(35, state); break;
        case 5: digitalWrite(34, state); break;
      }break;
    }
    case 2:{
      switch(y){
        case 0: digitalWrite(28, state); break;
        case 1: digitalWrite(29, state); break;
        case 2: digitalWrite(30, state); break;
        case 3: digitalWrite(31, state); break;
        case 4: digitalWrite(32, state); break;
        case 5: digitalWrite(33, state); break;
      }break;
    }
    case 3:{
      switch(y){
        case 0: digitalWrite(22, state); break;
        case 1: digitalWrite(23, state); break;
        case 2: digitalWrite(24, state); break;
        case 3: digitalWrite(25, state); break;
        case 4: digitalWrite(26, state); break;
        case 5: digitalWrite(27, state); break;
      }break;
    }
    case 4:{
      switch(y){
        case 0: digitalWrite(16, state); break;
        case 1: digitalWrite(17, state); break;
        case 2: digitalWrite(18, state); break;
        case 3: digitalWrite(19, state); break;
        case 4: digitalWrite(20, state); break;
        case 5: digitalWrite(21, state); break;
      }break;
    }
    case 5:{
      switch(y){
        case 0: digitalWrite(5, state); break;
        case 1: digitalWrite(4, state); break;
        case 2: digitalWrite(3, state); break;
        case 3: digitalWrite(2, state); break;
        case 4: digitalWrite(14, state); break;
        case 5: digitalWrite(15, state); break;
      }break;
    }
  }
}

void led_column_off(){
  for(int i=0; i<6; i++){
    for(int j=0; j<6; j++){
      led_column_control(i,j,LOW);
    }
  }
}

void led_plane_control(int plane, bool state){
  switch(plane){
    case 0:{
      digitalWrite(48, state);
      break;
    }
    case 1:{
      digitalWrite(49, state);
      break;
    }
    case 2:{
      digitalWrite(50, state);
      break;
    }
    case 3:{
      digitalWrite(51, state);
      break;
    }
    case 4:{
      digitalWrite(52, state);
      break;
    }
    case 5:{
      digitalWrite(53, state);
      break;
    }
    default:{
      digitalWrite(48, state);
      digitalWrite(49, state);
      digitalWrite(50, state);
      digitalWrite(51, state);
      digitalWrite(52, state);
      digitalWrite(53, state);
    }
  }
}

void led_column_select(int column, bool state){
  digitalWrite(column, state);
}

void led_plane_select(int plane){
  switch(plane){
    case 0:{
      digitalWrite(48, HIGH);
      digitalWrite(49, LOW);
      digitalWrite(50, LOW);
      digitalWrite(51, LOW);
      digitalWrite(52, LOW);
      digitalWrite(53, LOW);
      break;
    }
    case 1:{
      digitalWrite(48, LOW);
      digitalWrite(49, HIGH);
      digitalWrite(50, LOW);
      digitalWrite(51, LOW);
      digitalWrite(52, LOW);
      digitalWrite(53, LOW);
      break;
    }
    case 2:{
      digitalWrite(48, LOW);
      digitalWrite(49, LOW);
      digitalWrite(50, HIGH);
      digitalWrite(51, LOW);
      digitalWrite(52, LOW);
      digitalWrite(53, LOW);
      break;
    }
    case 3:{
      digitalWrite(48, LOW);
      digitalWrite(49, LOW);
      digitalWrite(50, LOW);
      digitalWrite(51, HIGH);
      digitalWrite(52, LOW);
      digitalWrite(53, LOW);
      break;
    }
    case 4:{
      digitalWrite(48, LOW);
      digitalWrite(49, LOW);
      digitalWrite(50, LOW);
      digitalWrite(51, LOW);
      digitalWrite(52, HIGH);
      digitalWrite(53, LOW);
      break;
    }
    case 5:{
      digitalWrite(48, LOW);
      digitalWrite(49, LOW);
      digitalWrite(50, LOW);
      digitalWrite(51, LOW);
      digitalWrite(52, LOW);
      digitalWrite(53, HIGH);
      break;
    }
    default:{
      digitalWrite(48, LOW);
      digitalWrite(49, LOW);
      digitalWrite(50, LOW);
      digitalWrite(51, LOW);
      digitalWrite(52, LOW);
      digitalWrite(53, LOW);
    }
  }
}

void control_all(bool state){
  for(int i=0; i<6; i++){
    if(state)
      led_plane_control(i,HIGH);
    else
      led_plane_control(i,LOW);
    for(int j=0; j<6; j++){
      led_column_control(i, j, state);
    }
  }
}

void column_demo(){
  for(int k=0; k<6; k++){
    for(int j=0; j<6; j++){
      led_column_control(k, j, HIGH);
      for(int i=0; i<6; i++){
        led_plane_select(i);
        delay(200);
      }
      led_column_control(k, j, LOW);
    }
  }
}

void alphabet(char alpha){
  unsigned long previousMillis = millis();
  int interval = 100;
  switch(alpha){
    case 'A':{
      cube_point snake_head = {5,1,0};
      cube_point snake_body[216]={{5,1,1},{5,1,2},{5,1,3},{5,5,0},{5,5,1},{5,5,2},{5,5,3},{5,2,2},{5,3,2},{5,4,2},{5,2,4},{5,3,4},{5,4,4}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 12);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
//            snake.head.x = 6;
            break;
          snake.head.x--;
          for(int j=0; j<13; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'B': led_cube_B(); break;
//    case 'C': led_cube_C(); break;
//    case 'D': led_cube_D(); break;
    case 'E':{
      cube_point snake_head = {5,1,0};
      cube_point snake_body[216]={{5,2,0},{5,3,0},{5,4,0},{5,1,4},{5,2,4},{5,3,4},{5,4,4},{5,4,1},{5,4,3},{5,2,2},{5,3,2},{5,4,2}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 11);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
//            snake.head.x = 6;
            break;
          snake.head.x--;
          for(int j=0; j<12; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'F': led_cube_F(); break;
    case 'G':{
      cube_point snake_head = {5,3,2};
      cube_point snake_body[216]={{5,2,2},{5,1,2},{5,1,1},{5,2,0},{5,3,0},{5,4,0},{5,5,1},{5,5,2},{5,5,3},{5,4,4},{5,3,4},{5,2,4}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 11);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
//            snake.head.x = 6;
            break;
          snake.head.x--;
          for(int j=0; j<12; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'H': led_cube_H(); break;
//    case 'I': led_cube_I(); break;
//    case 'J': led_cube_J(); break;
//    case 'K': led_cube_K(); break;
//    case 'L': led_cube_L(); break;
    case 'M':{
      cube_point snake_head = {5,1,0};
      cube_point snake_body[216]={{5,1,1},{5,1,2},{5,3,0},{5,3,1},{5,3,2},{5,5,0},{5,5,1},{5,5,2},{5,2,4},{5,4,4},{5,1,3},{5,3,3},{5,5,3}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 12);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
//            snake.head.x = 6;
            break;
          snake.head.x--;
          for(int j=0; j<13; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'N': led_cube_N(); break;
    case 'O':{
      cube_point snake_head = {5,2,0};
      cube_point snake_body[216]={{5,2,0},{5,3,0},{5,4,0},{5,5,1},{5,5,2},{5,5,3},{5,4,4},{5,3,4},{5,2,4},{5,1,3},{5,1,2},{5,1,1}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 11);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
//            snake.head.x = 6;
            break;
          snake.head.x--;
          for(int j=0; j<12; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'P': led_cube_P(); break;
//    case 'Q': led_cube_Q(); break;
    case 'R':{
      cube_point snake_head = {5,2,0};
      cube_point snake_body[216]={{5,5,0},{5,5,1},{5,5,2},{5,5,3},{5,5,4},{5,4,2},{5,4,4},{5,3,1},{5,3,2},{5,3,4},{5,2,3},{5,2,4}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 11);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
            break;
          snake.head.x--;
          for(int j=0; j<12; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'S': led_cube_S(); break;
//    case 'T': led_cube_T(); break;
//    case 'U': led_cube_U(); break;
    case 'V':{
      cube_point snake_head = {5,3,0};
      cube_point snake_body[216]={{5,1,3},{5,1,4},{5,2,1},{5,2,2},{5,4,1},{5,4,2},{5,5,3},{5,5,4}};
      bool led_cube_display[216];
      Snake snake(snake_head, snake_body, 7);
      while(1){
        plane_split(snake, led_cube_display);
        display_cube(led_cube_display);
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          if(snake.head.x==0)
            break;
          snake.head.x--;
          for(int j=0; j<9; j++){
            if(snake.body[j].x==0)
              snake.body[j].x = 6;
            snake.body[j].x--;
          }
          previousMillis = currentMillis;
        }
      }
      break;
    }
//    case 'W': led_cube_W(); break;
//    case 'X': led_cube_X(); break;
//    case 'Y': led_cube_Y(); break;
//    case 'Z': led_cube_Z(); break;
  }
}

//void coooooooool(){
//  bool led_cube_display[216];
//  unsigned long previousMillis = 0;
//  int interval = 100;
//  bool up_or_down[6]={1,0,0,0,0,0};      // Group 1 should go up, others go down
//  cube_point group_0[6] ={{0,0,0},{1,1,0},{2,2,0},{3,3,0},{4,4,0},{5,5,0}};
//  cube_point group_1[10]={{0,1,1},{1,2,1},{2,3,1},{3,4,1},{4,5,1},{1,0,1},{2,1,1},{3,2,1},{4,3,1},{5,4,1}};
//  cube_point group_2[8]={{0,2,2},{1,3,2},{2,4,2},{3,5,2},{2,0,2},{3,1,2},{4,2,2},{5,3,2}};
//  cube_point group_3[6]={{0,3,3},{1,4,3},{2,5,3},{3,0,3},{4,1,3},{5,2,3}};
//  cube_point group_4[4]={{0,4,4},{1,5,4},{4,0,4},{5,1,4}};
//  cube_point group_5[2]={{0,5,5},{5,0,5}};
//  
//  while(1){
//    for (int i=0; i<216; i++)
//      led_cube_display[i]=LOW;
//    general_plane_split(group_0, 6,  led_cube_display);
//    general_plane_split(group_1, 10, led_cube_display);
//    general_plane_split(group_2, 8,  led_cube_display);
//    general_plane_split(group_3, 6,  led_cube_display);
//    general_plane_split(group_4, 4,  led_cube_display);
//    general_plane_split(group_5, 2,  led_cube_display);
//    display_cube(led_cube_display);
//
//    unsigned long currentMillis = millis();
//    if (currentMillis - previousMillis >= interval) {
//      for(int i=0; i<6; i++){
//        if(up_or_down[0]==1)
//          group_0[i].z++;
//        else if(up_or_down[0]==0)
//          group_0[i].z--;
//      }
//      if(group_0[0].z==5)
//        up_or_down[0]=0;
//      if(group_0[0].z==0)
//        up_or_down[0]=1;
//
//      for(int i=0; i<10; i++){
//        if(up_or_down[1]==1)
//          group_1[i].z++;
//        else if(up_or_down[1]==0)
//          group_1[i].z--;
//      }
//      if(group_1[0].z==5)
//        up_or_down[1]=0;
//      if(group_1[0].z==0)
//        up_or_down[1]=1;
//
//      for(int i=0; i<8; i++){
//        if(up_or_down[2]==1)
//          group_2[i].z++;
//        else if(up_or_down[2]==0)
//          group_2[i].z--;
//      }
//      if(group_2[0].z==5)
//        up_or_down[2]=0;
//      if(group_2[0].z==0)
//        up_or_down[2]=1;
//
//      for(int i=0; i<6; i++){
//        if(up_or_down[3]==1)
//          group_3[i].z++;
//        else if(up_or_down[3]==0)
//          group_3[i].z--;
//      }
//      if(group_3[0].z==5)
//        up_or_down[3]=0;
//      if(group_3[0].z==0)
//        up_or_down[3]=1;
//
//      for(int i=0; i<4; i++){
//        if(up_or_down[4]==1)
//          group_4[i].z++;
//        else if(up_or_down[4]==0)
//          group_4[i].z--;
//      }
//      if(group_4[0].z==5)
//        up_or_down[4]=0;
//      if(group_4[0].z==0)
//        up_or_down[4]=1;
//
//      for(int i=0; i<2; i++){
//        if(up_or_down[5]==1)
//          group_5[i].z++;
//        else if(up_or_down[5]==0)
//          group_5[i].z--;
//      }
//      if(group_5[0].z==5)
//        up_or_down[5]=0;
//      if(group_5[0].z==0)
//        up_or_down[5]=1;
//      previousMillis = currentMillis;
//    }
//  }
//}

void superme(){
  bool led_cube_display[216];
  unsigned long previousMillis[6] = {0,0,0,0,0,0};
  int interval[6] = {100,120,140,160,180,200};
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

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis[0] >= interval[0]) {
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
      previousMillis[0] = currentMillis;
    }
    if (currentMillis - previousMillis[1] >= interval[1]) {
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
      previousMillis[1] = currentMillis;
    }
    if (currentMillis - previousMillis[2] >= interval[2]) {
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
      previousMillis[2] = currentMillis;
    }
    if (currentMillis - previousMillis[3] >= interval[3]) {
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
      previousMillis[3] = currentMillis;
    }
    if (currentMillis - previousMillis[4] >= interval[4]) {
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
      previousMillis[4] = currentMillis;
    }
    if (currentMillis - previousMillis[5] >= interval[5]) {
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
      previousMillis[5] = currentMillis;
    }
  }
}
