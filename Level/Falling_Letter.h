#pragma once

#include "Active_Brick.h"

enum ELetter_Type {
  ELT_None,

  ELT_O,    // Отмена действия других букв
  ELT_I,    // Инверсия
  ELT_C,    // Скорость
  ELT_M,    // Монстры
  ELT_G,    // Жизнь
  ELT_K,    // Клей
  ELT_W,    // Шире
  ELT_P,    // Пол
  ELT_L,    // Лазер
  ELT_T,    // Три
  ELT_Plus, // Переход на следующий уровень

};

enum EFalling_Letter_State {

  EFLS_Normal,
  EFLS_Finalizing, // Start to delete object
  EFLS_Finalized,  // End to delete object

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
  void Test_Draw_All_Steps(HDC hdc);

  const ELetter_Type Letter_Type;
  const EBrick_Type Brick_Type;

private:
  void Draw_Brick_Letter(HDC hdc);
  void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);

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
  static const int Max_Rotation_Step = 16;
  static const int Brick_Half_Height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
};