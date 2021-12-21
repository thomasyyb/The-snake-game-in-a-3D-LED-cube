#ifndef _SNAKE_CLASS
#define _SNAKE_CLASS

#include <SoftwareSerial.h>

typedef struct cube_point {
  int x;
  int y;
  int z;
}cube_point;

enum direction{
  Foreward,
  Backward,
  Upward,
  Downward,
  Leftward,
  Rightward
};

class Snake {
  public:
    cube_point head;
    cube_point* body;
    cube_point tail;    // tail saves the last node of the body
    int length;
    cube_point apple;
    enum direction snake_direction;
    Snake(cube_point x, cube_point* y, int z){
      head = x;
      body = y;
      length = z;
      snake_direction = Foreward;
    }
    void print_snake();
    void direction_change(enum direction input_direction);
    void snake_move();
    int collide_detection();
    bool hit_wall();
    bool hit_body();
    void apple_generator();
};

#endif
