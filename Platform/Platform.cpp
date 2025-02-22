#include "Platform.h"
#include <wingdi.h>

// AsPlatform
//------------------------------------------------------------------------------------------------------------

AsPlatform::~AsPlatform() { delete[] Normal_Platform_Image; }

AsPlatform::AsPlatform()
    : X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2),
      Platform_State(EPS_Normal), Inner_Width(Normal_Platform_Inner_Width),
      Rolling_Step(0), Normal_Platform_Image_Width(0),
      Normal_Platform_Image_Height(0), Normal_Platform_Image(0),
      Width(Normal_Width), Platform_Rect{}, Prev_Platform_Rect{},
      Highlight_Pen_Color{255, 255, 255}, Platform_Circle_Pen_Color{151, 0, 0},
      Platform_Inner_Pen_Color{0, 128, 192}, Highlight_Pen(0),
      Platform_Circle_Pen(0), Platform_Inner_Pen(0), Platform_Circle_Brush(0),
      Platform_Inner_Brush(0) {
  X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) {

  double inner_top_y;
  double inner_low_y;
  double inner_left_x;
  double inner_right_x;
  double inner_y;

  if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos) {
    return false;
  }

  // Reflect from the left edge of the platform
  if (Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball)) {
    return true;
  }

  // Reflect from the right edge of the platform
  if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - Circle_Size, ball)) {
    return true;
  }

  // The position of the center path of the platform's
  inner_top_y = (double)(AsConfig::Platform_Y_Pos + 1);
  inner_low_y = (double)(AsConfig::Platform_Y_Pos + Height - 1);
  inner_left_x = (double)(X_Pos + Circle_Size - 1);
  inner_right_x = (double)(X_Pos + Width - (Circle_Size - 1));
  double reflection_pos = 0;

  // Reflection from the center of the platform
  //  Check if ball is need to be reflected from the center part of the
  //  platform
  if (ball->Is_Moving_Up()) {
    inner_y = inner_low_y;
  } else {
    inner_y = inner_top_y;
  }

  // Check if the ball reflect from the bottom or top edge of the platform

  if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x,
                         inner_right_x, ball->Radius, reflection_pos)) {
    ball->Reflect(true);
    return true;
  }

  return false;
}

bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos,
                                   double fplatform_ball_x_offser,
                                   ABall *ball) {

  double dx;
  double dy;
  double distance;
  double two_radius;

  // angles
  double beta;
  double alfa;
  double gamma;

  // the center coordinates of the edges ball of the platform
  double platform_ball_x;
  double platform_ball_y;
  double platform_ball_radius;
  double related_ball_direction;
  // double moving_step_size = AsConfig::Global_Scale * 0.05;

  // The position of the edge of the platform
  platform_ball_radius = Circle_Size / 2.;
  platform_ball_x =
      (double)(X_Pos + platform_ball_radius) + fplatform_ball_x_offser;
  platform_ball_y = (double)(AsConfig::Platform_Y_Pos + platform_ball_radius);

  // Reflection from the  edges of the platform
  dx = next_x_pos - platform_ball_x;
  dy = next_y_pos - platform_ball_y;

  distance = sqrt(dx * dx + dy * dy);
  two_radius = platform_ball_radius + ball->Radius;

  if (fabs(distance - two_radius) < AsConfig::Moving_Step_Size) {

    // The ball is touch one of the edges of the platform

    beta = atan2(-dy, dx);

    related_ball_direction = ball->Get_Direction();
    related_ball_direction = related_ball_direction - beta;

    if (related_ball_direction > 2.0 * M_PI) {
      related_ball_direction = related_ball_direction - 2.0 * M_PI;
    }

    if (related_ball_direction < 0.) {
      related_ball_direction = related_ball_direction + 2.0 * M_PI;
    }

    if (related_ball_direction > M_PI_2 &&
        related_ball_direction < M_PI + M_PI_2) {
      alfa = beta + M_PI - ball->Get_Direction();
      gamma = beta + alfa;

      ball->Set_Direction(gamma);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init() {
  Highlight_Pen = CreatePen(PS_SOLID, 0, Highlight_Pen_Color.Get_RGB());

  AsConfig::Create_Pen_Brush(Platform_Circle_Pen_Color, Platform_Circle_Pen,
                             Platform_Circle_Brush);
  AsConfig::Create_Pen_Brush(Platform_Inner_Pen_Color, Platform_Inner_Pen,
                             Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act() {

  // if (AsConfig::Current_Timer_Tick % 10 != 0) {
  //   return;
  // }

  switch (Platform_State) {
  case EPS_Meltdown:
  case EPS_Roll_In:
  case EPS_Expand_Roll_In:
    Redraw_Platform();
  }
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State() { return Platform_State; }
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state) {
  int i, len;

  if (Platform_State == new_state)
    return;

  switch (new_state) {
  case EPS_Meltdown:
    len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

    for (i = 0; i < len; i++)
      Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;

    break;

  case EPS_Roll_In:
    X_Pos = AsConfig::Max_X_Pos - 1;
    Rolling_Step = Max_Rolling_Step - 1;
    break;
  }

  Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform() {
  int platform_width;

  Prev_Platform_Rect = Platform_Rect;

  if (Platform_State == EPS_Roll_In)
    platform_width = Circle_Size;
  else
    platform_width = Width;

  Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
  Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
  Platform_Rect.right =
      Platform_Rect.left + platform_width * AsConfig::Global_Scale;
  Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

  if (Platform_State == EPS_Meltdown)
    Prev_Platform_Rect.bottom =
        (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

  InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
  InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area) { // Рисуем платформу

  RECT intersection_rect;

  if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
    return;

  switch (Platform_State) {
  case EPS_Normal:
    Draw_Normal_State(hdc, paint_area);
    break;

  case EPS_Meltdown:
    Draw_Meltdown_State(hdc, paint_area);
    break;

  case EPS_Roll_In:
    Draw_Roll_In_State(hdc, paint_area);
    break;

  case EPS_Expand_Roll_In:
    Draw_Expanding_Roll_In_State(hdc, paint_area);
    break;
  }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc) { // Очищаем фоном прежнее место

  SelectObject(hdc, AsConfig::BG_Pen);
  SelectObject(hdc, AsConfig::BG_Brush);

  Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
            Prev_Platform_Rect.right - 1, Prev_Platform_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x,
                                       int y) { // Рисуем блик на шарике

  SelectObject(hdc, Highlight_Pen);

  Arc(hdc, x + AsConfig::Global_Scale, y + AsConfig::Global_Scale,
      x + (Circle_Size - 1) * AsConfig::Global_Scale - 1,
      y + (Circle_Size - 1) * AsConfig::Global_Scale - 1,
      x + 2 * AsConfig::Global_Scale, y + AsConfig::Global_Scale,
      x + AsConfig::Global_Scale, y + 3 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(
    HDC hdc, RECT &paint_area) { // Рисуем платформу в нормальном состоянии

  int x = X_Pos;
  int y = AsConfig::Platform_Y_Pos;
  int offset = 0;

  Clear_BG(hdc);

  // 1. Рисуем боковые шарики
  SelectObject(hdc, Platform_Circle_Pen);
  SelectObject(hdc, Platform_Circle_Brush);

  Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale,
          (x + Circle_Size) * AsConfig::Global_Scale - 1,
          (y + Circle_Size) * AsConfig::Global_Scale - 1);
  Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale,
          y * AsConfig::Global_Scale,
          (x + Circle_Size + Inner_Width) * AsConfig::Global_Scale - 1,
          (y + Circle_Size) * AsConfig::Global_Scale - 1);

  // 2. Рисуем блик
  Draw_Circle_Highlight(hdc, x * AsConfig::Global_Scale,
                        y * AsConfig::Global_Scale);

  // 3. Рисуем среднюю часть
  SelectObject(hdc, Platform_Inner_Pen);
  SelectObject(hdc, Platform_Inner_Brush);

  RoundRect(hdc, (x + 4) * AsConfig::Global_Scale,
            (y + 1) * AsConfig::Global_Scale,
            (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale - 1,
            (y + 1 + 5) * AsConfig::Global_Scale,
            3 * AsConfig::Global_Scale - 1, 3 * AsConfig::Global_Scale);

  x *= AsConfig::Global_Scale;
  y *= AsConfig::Global_Scale;

  if (Normal_Platform_Image == 0) {

    Normal_Platform_Image_Width = Width * AsConfig::Global_Scale;
    Normal_Platform_Image_Height = Height * AsConfig::Global_Scale;

    Normal_Platform_Image =
        new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];

    for (int i = 0; i < Normal_Platform_Image_Height; i++) {
      for (int j = 0; j < Normal_Platform_Image_Width; j++) {
        Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
      }
    }
  }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(
    HDC hdc,
    RECT &paint_area) { // Рисуем платформу в расплавленном состоянии

  int i, j;
  int x, y;
  int y_offset;

  int moved_columns_count = 0;
  int max_platform_y;
  int stroke_len;
  COLORREF pixel;
  COLORREF bg_pixel =
      RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

  HPEN color_pen;

  max_platform_y = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

  for (i = 0; i < Normal_Platform_Image_Width; i++) {
    if (Meltdown_Platform_Y_Pos[i] > max_platform_y)
      continue;

    ++moved_columns_count;

    y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
    x = Platform_Rect.left + i;

    y = Meltdown_Platform_Y_Pos[i];
    // set the init y position inside the Normal_Platform_Image array's of the
    // pixel
    j = 0;

    MoveToEx(hdc, x, y, 0);

    // get the stroke of one color in the current virtical line segment that
    // sets in the Normal_Platform_Image array
    while (Get_Platform_Image_Stroke_Color(i, j, color_pen, stroke_len)) {
      SelectObject(hdc, color_pen);
      LineTo(hdc, x, y + stroke_len);
      y += stroke_len;
      j += stroke_len;
    }

    // Clear the background above the stroke
    y = Meltdown_Platform_Y_Pos[i];
    MoveToEx(hdc, x, y, 0);
    SelectObject(hdc, AsConfig::BG_Pen);
    LineTo(hdc, x, y + y_offset);

    Meltdown_Platform_Y_Pos[i] += y_offset;
  }

  if (moved_columns_count == 0)
    Platform_State = EPS_Missing; // Вся платформа сдвинулась за пределы окна
}

// Function to return the colour's and length the current stroke of the platform
bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, HPEN &color_pen,
                                                 int &stroke_len) {
  int color;
  // get the current pixel position in the platform pixel's array
  int offset = y * Normal_Platform_Image_Width + x;
  stroke_len = 0;

  if (y >= Normal_Platform_Image_Height) {
    return false; // Out of image boundary
  }

  for (int i = y; i < Normal_Platform_Image_Height; i++) {

    if (i == y) {
      color = Normal_Platform_Image[offset];
      stroke_len = 1;
    }

    else {
      if (color == Normal_Platform_Image[offset]) {
        ++stroke_len;
      } else {
        break;
      }
    }
    // Normal_Platform_Image save the pixels in
    // the line, but we need a vertical pixel
    offset += Normal_Platform_Image_Width;
  }

  if (color == Highlight_Pen_Color.Get_RGB()) {
    color_pen = Highlight_Pen;

  } else if (color == Platform_Circle_Pen_Color.Get_RGB()) {
    color_pen = Platform_Circle_Pen;

  } else if (color == Platform_Inner_Pen_Color.Get_RGB()) {
    color_pen = Platform_Inner_Pen;

  } else if (color == AsConfig::BG_Color.Get_RGB()) {
    color_pen = AsConfig::BG_Pen;
  } else {
    color_pen = 0;
  }

  return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(
    HDC hdc, RECT &paint_area) { // Рисуем выкатывающуюся платформу

  int x = X_Pos * AsConfig::Global_Scale;
  int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
  int roller_size = Circle_Size * AsConfig::Global_Scale;
  double alpha;
  XFORM xform, old_xform;

  Clear_BG(hdc);

  // 1. Шарик
  SelectObject(hdc, Platform_Circle_Pen);
  SelectObject(hdc, Platform_Circle_Brush);

  Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size - 1);

  // 2. Разделительная линия

  alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step;

  xform.eM11 = (float)cos(alpha);
  xform.eM12 = (float)sin(alpha);
  xform.eM21 = (float)-sin(alpha);
  xform.eM22 = (float)cos(alpha);
  xform.eDx = (float)(x + roller_size / 2.);
  xform.eDy = (float)(y + roller_size / 2.);
  GetWorldTransform(hdc, &old_xform);
  SetWorldTransform(hdc, &xform);

  SelectObject(hdc, AsConfig::BG_Pen);
  SelectObject(hdc, AsConfig::BG_Brush);

  Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2,
            AsConfig::Global_Scale / 2 - 1, roller_size / 2 - 1);

  SetWorldTransform(hdc, &old_xform);

  // 3. Блик
  Draw_Circle_Highlight(hdc, x, y);

  ++Rolling_Step;

  if (Rolling_Step >= Max_Rolling_Step)
    Rolling_Step -= Max_Rolling_Step;

  X_Pos -= Rolling_Platform_Speed;

  if (X_Pos <= Roll_In_Platform_End_X_Pos) {
    X_Pos += Rolling_Platform_Speed;
    Platform_State = EPS_Expand_Roll_In;
    Inner_Width = 1;
  }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Roll_In_State(
    HDC hdc,
    RECT &paint_area) { // Рисуем расширяющуюся после выкатывания платформу

  Draw_Normal_State(hdc, paint_area);

  --X_Pos;
  Inner_Width += 2;

  if (Inner_Width >= Normal_Platform_Inner_Width) {
    Inner_Width = Normal_Platform_Inner_Width;
    Platform_State = EPS_Ready;
    Redraw_Platform();
  }
}
//------------------------------------------------------------------------------------------------------------
