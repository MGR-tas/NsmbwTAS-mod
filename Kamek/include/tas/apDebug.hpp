#pragma once

#include <common.h>
#include <game.h>
#include <profile.h>
#include <g3dhax.h>

extern bool InputDispToggle;
extern int HitboxFormat;
extern bool showGrid;
extern int ShowInfo;

class APDebugDrawer : public m3d::proc_c {
	public:
		APDebugDrawer();
		void setMeUp();
		void drawMe();
		void drawOpa();
		void drawXlu();

		bool amISetUp;
		mHeapAllocator_c allocator;
};

static APDebugDrawer defaultInstance;
extern bool enableDebugMode;

int APDebugDraw();

void drawInfoBox(float posX, float posY, u8 rows, u32 length, float scale);
void drawInputDisplay(float baseX, float baseY, float dispScale);
void drawTilesetOverlay();
void drawHitboxes();
