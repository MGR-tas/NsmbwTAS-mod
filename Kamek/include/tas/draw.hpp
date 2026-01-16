#pragma once
#include <common.h>

void DrawPoint(float x, float y, float z, u8 r, u8 g, u8 b, u8 a);
void DrawLine(float x1, float y1, float x2, float y2, float z, u8 r, u8 g, u8 b, u8 a);
void DrawQuad(
    float tlX, float tlY, float trX, float trY, float blX, float blY, float brX, float brY,
    float z, u8 r, u8 g, u8 b, u8 a
);
void DrawCircle(
    float centreX, float centreY, float radiusX, float radiusY,
    float z, u8 r, u8 g, u8 b, u8 a
);
void DrawPartialCircle(
    float centreX, float centreY, float radius, float z,
    u16 minAngle, u16 maxAngle, u8 r, u8 g, u8 b, u8 a
);
float drawFloat(
    float num, float posX, float posY, float scale,
    u32 maxLength, u8 maxDigitsR, u8 r, u8 g, u8 b, u8 a
);
