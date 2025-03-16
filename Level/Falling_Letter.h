#pragma once

#include "Active_Brick.h"

enum ELetter_Type {
  ELT_None,

  ELT_O
};


enum EFalling_Letter_State {

  EFLS_Normal,
  EFLS_Finalizing,  //Start to delete object
  EFLS_Finalized, //End to delete object

};

class AFalling_Letter : public AGraphics_Object {

public:
  AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x,
                  int y);
  ~AFalling_Letter() override;

  void Act() override;
  void Draw(HDC hdc, RECT &paint_area) override;
  bool Is_Finished() override;

  void Get_Letter_Cell(RECT &rect);
  void Finalize();

  const ELetter_Type Letter_Type;
  const EBrick_Type Brick_Type;

private:
  void Draw_Brick_Letter(HDC hdc);

  void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen,
                               HBRUSH &front_brush, HPEN &back_pen,
                               HBRUSH &back_brush);

  RECT Letter_Cell, Prev_Letter_Cell;

  EFalling_Letter_State Falling_Letter_State;
  int X;
  int Y;
  int Rotation_Step;
  int Next_Rotation_Step;

  static const int Ticks_Per_Step = 4;
};