#include "snake_class.hh"
#include <SoftwareSerial.h>

void Snake::direction_change(enum direction input_direction){
  if((snake_direction==Foreward  && input_direction==Backward )||\
     (snake_direction==Backward  && input_direction==Foreward )||\
     (snake_direction==Upward    && input_direction==Downward )||\
     (snake_direction==Downward  && input_direction==Upward   )||\
     (snake_direction==Leftward  && input_direction==Rightward)||\
     (snake_direction==Rightward && input_direction==Leftward ))
    return;
  else
    snake_direction=input_direction;
}

void Snake::snake_move(){
  tail = body[length];  // save the tail for the apple
  for(int i=length; i>0; i--){
    body[i]=body[i-1];
  }
  body[0]=head;
  switch(snake_direction){
    case Foreward:  head.x++; break;
    case Backward:  head.x--; break;
    case Upward:    head.z++; break;
    case Downward:  head.z--; break;
    case Leftward:  head.y++; break;
    case Rightward: head.y--; break;
  }
}

//void Snake::snake_automove(){
//  snake_move
//}

int Snake::collide_detection(){
  if(head.x==apple.x && head.y==apple.y && head.z==apple.z){
    length++;
    body[length]=tail;  // make the snake longer by adding the tail
    apple_generator();  // generate a new apple
    return 1;  // eat apple
  }else if(hit_body() || hit_wall()){
    return 2;  // game over
  }else{
    return 0;  // nothing happens
  }
}

bool Snake::hit_wall(){
  if(head.x>5 || head.x<0 || head.y>5 || head.y<0 || head.z>5 || head.z<0){
    return 1;
  }
  else
    return 0;
}

bool Snake::hit_body(){
  for(int i=0; i<=length; i++){
    if(head.x == body[i].x && head.y == body[i].y && head.z == body[i].z){
      return 1;
    }
  }
  return 0;
}

void Snake::apple_generator(){
  apple.x = random()%6;
  apple.y = random()%6;
  apple.z = random()%6;
  if(apple.x==3 && apple.y==3 && apple.z==2){
    apple.x = random()%6;
    apple.y = random()%6;
    apple.z = random()%6;
  }
}
