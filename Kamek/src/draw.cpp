#include <tas/draw.hpp>
#include <game.h>

void DrawPoint(float posX, float posY, float z, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_POINTS, GX_VTXFMT0, 1);
	GXPosition3f32(posX, posY, z);
	GXColor4u8(r,g,b,a);
    GXEnd();
}

void DrawLine(float x1, float y1, float x2, float y2, float z, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_LINES, 0, 2);
    GXPosition3f32(x1, y1, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(x2, y2, z);
    GXColor4u8(r,g,b,a);
    GXEnd();
}

void DrawQuad(float tlX, float tlY, float trX, float trY, float blX, float blY, float brX, float brY, float z, u8 r, u8 g, u8 b, u8 a) {
    // Setup drawing
    GXBegin(GX_LINES, 0, 8);

    // Draw top
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);

    // Draw left
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);

    // Draw right
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    // Draw bottom
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    GXEnd();
}

void DrawCircle(float centreX, float centreY, float radiusX, float radiusY, float z, u8 r, u8 g, u8 b, u8 a) {
    // Define a few variables
    const int numVert = 64;
    const float step = 256.0f/numVert;
    float sin, cos, xDist, yDist;

    // Initialize the prev variables
    float prevXDist = radiusX;
    float prevYDist = 0.0f;

    // Begin drawing
    GXBegin(GXPrimitive::LINES, 0, numVert * 2);

    // Draw each line
    for (int i = 1; i <= numVert / 4; i++) {

        // Calculate sin and cos for the current angle
        nw4r::math::SinCosFIdx(&sin, &cos, step * i);

        // Calculate the distances from the center
        xDist = radiusX * cos;
        yDist = radiusY * sin;

        // Draw on the first quadrant
        GXPosition3f32(centreX + prevXDist, centreY + prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + xDist, centreY + yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the second quadrant
        GXPosition3f32(centreX - prevYDist, centreY + prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - yDist, centreY + xDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the third quadrant
        GXPosition3f32(centreX - prevXDist, centreY - prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - xDist, centreY - yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the fourth quadrant
        GXPosition3f32(centreX + prevYDist, centreY - prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + yDist, centreY - xDist, z);
        GXColor4u8(r,g,b,a);

        // Override the "previous" values
        prevXDist = xDist;
        prevYDist = yDist;
    }

    // End drawing
    GXEnd();
}

void DrawPartialCircle(float centreX, float centreY, float radius, float z, u16 minAngle, u16 maxAngle, u8 r, u8 g, u8 b, u8 a) {
    // Initialize variables
    const int maxVerts = 64;
    const float step = 256.0f/maxVerts;
    float sin, cos, xDist, yDist;

    // Calculate the number of required vertices and the starting angle
    u16 totalAngle = (maxAngle - minAngle) & 0xFFFF;
    int numVert = (totalAngle * maxVerts / 0x10000);
    float startAngle = minAngle / 256.0f;

    // Set up the prev variables
    nw4r::math::SinCosFIdx(&sin, &cos, startAngle);
    float prevXDist = radius * cos;
    float prevYDist = radius * sin;

    // Begin drawing
    GXBegin(GXPrimitive::LINES, 0, numVert * 2);

    // Draw each line
    for (int i = 1; i <= numVert; i++) {

        // Calculate sin and cos for the current angle
        nw4r::math::SinCosFIdx(&sin, &cos, startAngle + step * i);

        // Calculate the distances from the center
        xDist = radius * cos;
        yDist = radius * sin;

        // Draw the line
        GXPosition3f32(centreX + prevXDist, centreY + prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + xDist, centreY + yDist, z);
        GXColor4u8(r,g,b,a);

        // Override the "previous" values
        prevXDist = xDist;
        prevYDist = yDist;
    }
}

float drawFloat(float num, float posX, float posY, float scale, u32 maxLength, u8 maxDigitsR, u8 r, u8 g, u8 b, u8 a) {
    const float z = 9000.0f;

	float basePosX = posX;
	GXSetLineWidth(6, GX_TO_ZERO);
	
	if (num < 0) {
		//posX -= 4*scale;
		DrawLine(posX+0.5*scale, posY-2*scale, posX+2*scale, posY-2*scale, z, r, g, b, a);
	}
	posX += 3*scale;
	
	float manipNum = abs(num);
	int digitsL = 0;
	float placeValue = 1;
	
	//determine how many digits are to the left of the decimal
	do {
		digitsL++;
		placeValue *= 10;
	} while (manipNum >= placeValue);
	
	int i = digitsL+maxDigitsR;
	
	do {
		placeValue /= 10;
		digitsL--;
		i--;
		if (manipNum >= 9*placeValue) {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 9*placeValue;
		} else if (manipNum >= 8*placeValue) {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY, posX, posY-4*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 8*placeValue;
		} else if (manipNum >= 7*placeValue) {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX, posY-4*scale, z, r, g, b, a);
			manipNum -= 7*placeValue;
		} else if (manipNum >= 6*placeValue) {
			DrawLine(posX+1*scale, posY, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-2*scale, posX, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX+2, posY-4*scale, posX+2*scale, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			manipNum -= 6*placeValue;
		} else if (manipNum >= 5*placeValue) {
			DrawLine(posX+2*scale, posY, posX, posY, z, r, g, b, a);
			DrawLine(posX, posY, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 5*placeValue;
		} else if (manipNum >= 4*placeValue) {
			DrawLine(posX, posY, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-2*scale, posX+2*scale, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 4*placeValue;
		} else if (manipNum >= 3*placeValue) {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 3*placeValue;
		} else if (manipNum >= 2*placeValue) {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2*scale, posY-2*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY-2*scale, posX, posY-2*scale, z, r, g, b, a);
			DrawLine(posX, posY-2*scale, posX, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 2*placeValue;
		} else if (manipNum >= 1*placeValue) {
			DrawLine(posX, posY-1*scale, posX+1*scale, posY, z, r, g, b, a);
			DrawLine(posX+1*scale, posY, posX+1, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			manipNum -= 1*placeValue;
		} else {
			DrawLine(posX, posY, posX+2*scale, posY, z, r, g, b, a);
			DrawLine(posX, posY-4*scale, posX+2, posY-4*scale, z, r, g, b, a);
			DrawLine(posX, posY, posX, posY-4*scale, z, r, g, b, a);
			DrawLine(posX+2*scale, posY, posX+2, posY-4*scale, z, r, g, b, a);
		}
		posX += 4*scale;
		
		if (digitsL == 0 and maxDigitsR > 0) {
			DrawLine(posX-0.5*scale, posY-4*scale, posX-0.5*scale, posY-3.5*scale, z, r, g, b, a);
			posX += 1*scale;
		}
		
		if (manipNum == 0 and maxDigitsR != 0)
			i = digitsL + 1;
		
	} while (i > 0 and posX < basePosX+(maxLength-2)*scale);
	return posX;
}
