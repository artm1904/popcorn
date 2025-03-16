#pragma once

#include "../Ball/Ball.h"

#include "Falling_Letter.h"




//------------------------------------------------------------------------------------------------------------
class AsLevel : public AHit_Checker {
public:
  AsLevel();

  virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

  void Init();
  void
  Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width]);
  void Act();
  void Act_Objects(AGraphics_Object **objects_array, int object_max_count);
  void Draw(HDC hdc, RECT &paint_area);


  bool Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter);

  // AActive_Brick Active_Brick;
  static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
  static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

private:
  void Draw_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array,
                    int object_max_count);
  bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x,
                          int level_y, ABall *ball, double &reflection_pos);
  bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x,
                            int level_y, ABall *ball, double &reflection_pos);
  void On_Hit(int brick_x, int brick_y);
  bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
  void Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type);

  void Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);
  void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen,
                               HBRUSH &front_brush, HPEN &back_pen,
                               HBRUSH &back_brush);
  void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type,
                         ELetter_Type letter_type, int rotation_step);

  RECT Level_Rect;

  double Current_Brick_Left_X, Current_Brick_Right_X;
  double Current_Brick_Top_Y, Current_Brick_Low_Y;

  char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];
  AActive_Brick *Active_Bricks[AsConfig::Max_Active_Bricks_Count];
  int Active_Bricks_Count;
  AFalling_Letter *Falling_Letters[AsConfig::Max_Falling_Letters_Count];
  int Falling_Letters_Count;
};
//------------------------------------------------------------------------------------------------------------
