#include <tas/apDebug.hpp>
#include <tas/draw.hpp>
#include <common.h>
#include <game.h>
#include <profile.h>
#include <g3dhax.h>

bool InputDispToggle = true;
int HitboxFormat = 1;
bool showGrid = false;
int ShowInfo = 0;
bool enableDebugMode = true;

int APDebugDraw() {
	int pressedButtons = GetActiveRemocon()->nowPressed;
	int heldButtons = GetActiveRemocon()->heldButtons;
	if ((heldButtons & WPAD_ONE) && (pressedButtons & WPAD_MINUS))
		InputDispToggle = !InputDispToggle;
	if ((heldButtons & WPAD_TWO) && (pressedButtons & WPAD_MINUS)) {
		HitboxFormat++;
		if (HitboxFormat == 2)  //only cycle between basic and off; TASing can turn on advanced hitboxes with Studio
			HitboxFormat = 0;
	}
	if ((heldButtons & WPAD_DOWN) && (pressedButtons & WPAD_MINUS)) {
		ShowInfo++;
		if (ShowInfo == 3)
			ShowInfo = 0;
	}
	if ((heldButtons & WPAD_UP) && (pressedButtons & WPAD_MINUS))
		showGrid = !showGrid;
	if (enableDebugMode)
		defaultInstance.drawMe();
	return 1;
}


APDebugDrawer::APDebugDrawer() {
	amISetUp = false;
}

void APDebugDrawer::setMeUp() {
	allocator.setup(GameHeaps[0], 0x20);
	setup(&allocator);
}

void APDebugDrawer::drawMe() {
	if (!amISetUp) {
		setMeUp();
		amISetUp = true;
	}

	scheduleForDrawing();
}


void drawInfoBox(float posX, float posY, u8 rows, u32 length, float scale) {
	const float z = 9000.0f;
	float height = 6*scale;
	GXSetLineWidth(120, GX_TO_ZERO);
	DrawLine(posX, posY-height/2+1*scale, posX+((length+1)*scale), posY-height/2+1*scale, z, 0x00, 0x00, 0x00, 0xFF);
	
	//posY = posY - 1*scale;
	u8 i = 0;
	
	//GXSetLineWidth(120, GX_TO_ZERO);
	do {
		DrawLine(posX, posY-height/2-1*scale, posX+((length+1)*scale), posY-height/2-1*scale, z, 0x00, 0x00, 0x00, 0xFF);
	
		DrawLine(posX+1*scale, posY-height/2, posX+(length*scale), posY-height/2, z, 0xFF, 0xFF, 0xFF, 0xFF);
		posY = posY - height;
		i = i + 1;
	} while (i < rows);
	
}

void drawInputDisplay(float baseX, float baseY, float dispScale) {
	// Input Display	
	if (InputDispToggle == false)  //only draw if input display enabled
	return;
	int pressedButtons = GetActiveRemocon()->nowPressed;
	int heldButtons = GetActiveRemocon()->heldButtons;

	//OSReport("Camera Data : cameraLeft = %f, cameraWidth = %f, cameraTop = %f, cameraHeight = %f\n", screenLeft, screenWidth, screenTop, screenHeight);	  	  

	const float z = 9000.0f;

	// D-pad
	GXSetLineWidth(84, GX_TO_ZERO);
	DrawLine( 19*dispScale+baseX, 10*dispScale+baseY, 1*dispScale+baseX, 10*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	GXSetLineWidth(72, GX_TO_ZERO);
	DrawLine( 10*dispScale+baseX, 1*dispScale+baseY, 10*dispScale+baseX, 19*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	GXSetLineWidth(48, GX_TO_ZERO);
	DrawLine( 10*dispScale+baseX, 2*dispScale+baseY, 10*dispScale+baseX, 18*dispScale+baseY, z, 0xFF,0xFF,0xFF,0xFF);
	GXSetLineWidth(60, GX_TO_ZERO);
	DrawLine( 18*dispScale+baseX, 10*dispScale+baseY, 2*dispScale+baseX, 10*dispScale+baseY, z, 0xFF,0xFF,0xFF,0xFF);

	// Buttons
	GXSetLineWidth(72, GX_TO_ZERO);
	DrawLine( 30*dispScale+baseX, 7*dispScale+baseY, 30*dispScale+baseX, 13*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	GXSetLineWidth(48, GX_TO_ZERO);
	DrawLine( 30*dispScale+baseX, 8*dispScale+baseY, 30*dispScale+baseX, 12*dispScale+baseY, z, 0xFF,0xFF,0xFF,0xFF);
	GXSetLineWidth(72, GX_TO_ZERO);	
	DrawLine( 42*dispScale+baseX, 7*dispScale+baseY, 42*dispScale+baseX, 13*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	GXSetLineWidth(48, GX_TO_ZERO);
	DrawLine( 42*dispScale+baseX, 8*dispScale+baseY, 42*dispScale+baseX, 12*dispScale+baseY, z, 0xFF,0xFF,0xFF,0xFF);


	// draw input
	
	//OSReport("    wiimoteMoveDistanceOrSomething %f\nlastWiimoteMoveDistanceOrSomething%f\n", GetActiveRemocon()->wiimoteMoveDistanceOrSomething, GetActiveRemocon()->lastWiimoteMoveDistanceOrSomething);
	
	if (GetActiveRemocon()->moveDistance != GetActiveRemocon()->lastMoveDistance) {
	  	GXSetLineWidth(24, GX_TO_ZERO);
	    if (GetActiveRemocon()->isShaking == 0) {
		DrawLine( 50*dispScale+baseX, 2*dispScale+baseY, 50*dispScale+baseX, 18*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
		DrawLine( baseX-3*dispScale, 2*dispScale+baseY, baseX-3*dispScale, 18*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	    } else {
		DrawLine( 50*dispScale+baseX, 2*dispScale+baseY, 50*dispScale+baseX, 18*dispScale+baseY, z, 0x00,0xFF,0x00,0xFF);
		DrawLine( baseX-3*dispScale, 2*dispScale+baseY, baseX-3*dispScale, 18*dispScale+baseY, z, 0x00,0xFF,0x00,0xFF);
	    }
	}
	  
	if (heldButtons & WPAD_ONE) {
		GXSetLineWidth(48, GX_TO_ZERO);
		DrawLine( 30*dispScale+baseX, 8*dispScale+baseY, 30*dispScale+baseX, 12*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
	if ((GetActiveRemocon()->controllerType == 1) && (heldButtons & WPAD_B)) {
	  	GXSetLineWidth(48, GX_TO_ZERO);
		DrawLine( 30*dispScale+baseX, 8*dispScale+baseY, 30*dispScale+baseX, 12*dispScale+baseY, z, 0x00,0xFF,0x00,0xFF);
	}
	if (heldButtons & WPAD_TWO) {
		GXSetLineWidth(48, GX_TO_ZERO);
		DrawLine( 42*dispScale+baseX, 8*dispScale+baseY, 42*dispScale+baseX, 12*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
	if (heldButtons & WPAD_LEFT) {
		GXSetLineWidth(60, GX_TO_ZERO);
		DrawLine( 10*dispScale+baseX, 10*dispScale+baseY, 2*dispScale+baseX, 10*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
	if (heldButtons & WPAD_RIGHT) {
		GXSetLineWidth(60, GX_TO_ZERO);
		DrawLine( 10*dispScale+baseX, 10*dispScale+baseY, 18*dispScale+baseX, 10*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
	if (heldButtons & WPAD_UP) {
		GXSetLineWidth(48, GX_TO_ZERO);
		DrawLine( 10*dispScale+baseX, 10*dispScale+baseY, 10*dispScale+baseX, 18*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
	if (heldButtons & WPAD_DOWN) {
		GXSetLineWidth(48, GX_TO_ZERO);
		DrawLine( 10*dispScale+baseX, 10*dispScale+baseY, 10*dispScale+baseX, 2*dispScale+baseY, z, 0xFF,0x00,0x00,0xFF);
	}
		  
	// lines
	GXSetLineWidth(12, GX_TO_ZERO);
	DrawLine( 6*dispScale+baseX, 10*dispScale+baseY, 3*dispScale+baseX, 10*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	DrawLine( 14*dispScale+baseX, 10*dispScale+baseY, 17*dispScale+baseX, 10*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	GXSetLineWidth(10, GX_TO_ZERO);
	DrawLine( 10*dispScale+baseX, 6*dispScale+baseY, 10*dispScale+baseX, 3*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);
	DrawLine( 10*dispScale+baseX, 14*dispScale+baseY, 10*dispScale+baseX, 17*dispScale+baseY, z, 0x00,0x00,0x00,0xFF);

	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(18, GX_TO_ZERO);
}

void drawTilesetOverlay() {
	if (!HitboxFormat) return;
	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(18, GX_TO_ZERO);

	float tilesetBaseX = ClassWithCameraInfo::instance->screenLeft;
	float tilesetBaseY = ClassWithCameraInfo::instance->screenTop + 16;
	int tilesetDispColumns = ClassWithCameraInfo::instance->screenWidth/16 + 1;
	int tilesetDispRows = ClassWithCameraInfo::instance->screenHeight/16 + 1;
	int tilesetDispXI = 0;
	int tilesetDispYI = 0;
	u8 r = 0xFF;
	u8 g = 0x7F;
	u8 b = 0x50;
	u8 a = 0xFF;
	int borderEx = 0x60;

	const float z = 9000.0f;

	while (tilesetDispYI <= tilesetDispRows) {
		while (tilesetDispXI <= tilesetDispColumns) {
			float tileDispRefX = (floor(tilesetBaseX/16)+tilesetDispXI)*16;
			float tileDispRefY = (floor(tilesetBaseY/16)-tilesetDispYI)*16;
			u32 curTile = collisionMgr_c::getTileBehaviour1At(tileDispRefX, tileDispRefY, 0);
			u16 curTileB = collisionMgr_c::getTileBehaviour2At(tileDispRefX, tileDispRefY, 0);					
			
			if (showGrid == true) {  //only draw bottom and right lines so top/left tile lines don't get overwritten
				DrawLine(tileDispRefX+16, tileDispRefY-16, tileDispRefX, tileDispRefY-16, z, 0x93, 0x70, 0xDB, 0xFF);
				DrawLine(tileDispRefX+16, tileDispRefY, tileDispRefX+16, tileDispRefY-16, z, 0x93, 0x70, 0xDB, 0xFF);
			}
			
			if (HitboxFormat == 2 && (!(curTile & (borderEx+0x1+0x20000+0x8000+0x80000)) || curTile & 0x4) && curTile != 0)
				DrawQuad(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY, tileDispRefX, tileDispRefY-16, tileDispRefX+16, tileDispRefY-16, z, r, g, b, a);
			
			if (curTile & 0x1 || curTile & 0x2000 || curTile & 0x8 || curTile & 0x10) {  // solid blocks
				if ((collisionMgr_c::getTileBehaviour1At(tileDispRefX, tileDispRefY+16, 0) != curTile) && !(collisionMgr_c::getTileBehaviour1At(tileDispRefX, tileDispRefY+16, 0) & borderEx))
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY, z, r, g, b, a);
				if ((collisionMgr_c::getTileBehaviour1At(tileDispRefX+16, tileDispRefY, 0) != curTile) && !(collisionMgr_c::getTileBehaviour1At(tileDispRefX+16, tileDispRefY, 0) & borderEx))
					DrawLine(tileDispRefX+16, tileDispRefY, tileDispRefX+16, tileDispRefY-16, z, r, g, b, a);
				if ((collisionMgr_c::getTileBehaviour1At(tileDispRefX, tileDispRefY-16, 0) != curTile) && !(collisionMgr_c::getTileBehaviour1At(tileDispRefX, tileDispRefY-16, 0) & borderEx))
					DrawLine(tileDispRefX+16, tileDispRefY-16, tileDispRefX, tileDispRefY-16, z, r, g, b, a);
				if ((collisionMgr_c::getTileBehaviour1At(tileDispRefX-16, tileDispRefY, 0) != curTile) && !(collisionMgr_c::getTileBehaviour1At(tileDispRefX-16, tileDispRefY, 0) & borderEx))
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX, tileDispRefY, z, r, g, b, a);
			}
			if (curTile & 0x20) {  // slope
				if (curTileB == 0x0)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+16, tileDispRefY, z, r, g, b, a);
				if (curTileB == 0x1)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0x2)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+32, tileDispRefY, z, r, g, b, a);
				if (curTileB == 0x4)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+32, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0xB)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+64, tileDispRefY, z, r, g, b, a);
				if (curTileB == 0xF)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+64, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0xA && HitboxFormat == 2)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY, z, r, g, b, a);
					//DrawQuad(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY, tileDispRefX, tileDispRefY-16, tileDispRefX+16, tileDispRefY-16, z, r, g, b, a);
			}
			if (curTile & 0x40) {  // inverse slope
				if (curTileB == 0x0)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0x1)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+16, tileDispRefY, z, r, g, b, a);
				if (curTileB == 0x2)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+32, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0x4)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+32, tileDispRefY, z, r, g, b, a);
				if (curTileB == 0xB)
					DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+64, tileDispRefY-16, z, r, g, b, a);
				if (curTileB == 0xF)
					DrawLine(tileDispRefX, tileDispRefY-16, tileDispRefX+64, tileDispRefY, z, r, g, b, a);
			}
			if (curTile & 0x8000 || curTile & 0x20000 || curTile & 0x80000)  // solid-on-top platforms	
				DrawLine(tileDispRefX, tileDispRefY, tileDispRefX+16, tileDispRefY, z, r, g, b, a);
			
			tilesetDispXI = tilesetDispXI + 1;
		}
		tilesetDispXI = 0;
		tilesetDispYI = tilesetDispYI + 1;
	}
}

void drawHitboxes() {
	if (!HitboxFormat) return;

	const float z = 9000.0f;

	Physics *p = Physics::globalListHead;
	while (p) {
		u32 uptr = (u32)p;
		u8 r = 0xFF;
		u8 g = 0x7F;
		u8 b = 0x50;
		u8 a = 0xFF;

		float tlX = p->unkArray[0].x;
		float tlY = p->unkArray[0].y;
		float trX = p->unkArray[3].x;
		float trY = p->unkArray[3].y;
		float blX = p->unkArray[1].x;
		float blY = p->unkArray[1].y;
		float brX = p->unkArray[2].x;
		float brY = p->unkArray[2].y;

		DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, z, r, g, b, a);

		p = p->next;
	}
	// Basic Colliders
	BasicCollider *bc = BasicCollider::globalListHead;
	while (bc) {
		u32 uptr = (u32)bc;
		u8 r = 0x00;
		u8 g = 0x00;
		u8 b = 0xFF;
		u8 a = 0xFF;

		switch (bc->type) {
			case 0: case 2:
				DrawLine(bc->leftX, bc->leftY, bc->rightX, bc->rightY, z, r, g, b, a);
				break;
		}

		bc = bc->next;
	}
	
	ActivePhysics *ap = ActivePhysics::globalListHead;
	//active physics			
	while (ap) {

		float centreX = ap->owner->pos.x + ap->info.xDistToCenter;
		float centreY = ap->owner->pos.y + ap->info.yDistToCenter;
		float edgeDistX = ap->info.xDistToEdge;
		float edgeDistY = ap->info.yDistToEdge;

		float tlX = centreX - edgeDistX, tlY = centreY + edgeDistY;
		float trX = centreX + edgeDistX, trY = centreY + edgeDistY;

		float blX = centreX - edgeDistX, blY = centreY - edgeDistY;
		float brX = centreX + edgeDistX, brY = centreY - edgeDistY;
		
		if (ShowInfo == 1) {
			float boxX = tlX - 4;
			float boxY = trY + 16;
			float boxScale = ClassWithCameraInfo::instance->screenHeight/250;
			u32 boxLength = -1;
			boxX = drawFloat(ap->owner->pos_delta.x, boxX, boxY, boxScale, boxLength, 4, 0x00, 0x00, 0x00, 0xFF) + 1;
			DrawLine(boxX-0.5*boxScale, boxY-4*boxScale, boxX, boxY-3.25*boxScale, z, 0x00, 0x00, 0x00, 0xFF);
			drawFloat(ap->owner->pos_delta.y, boxX, boxY, boxScale, boxLength, 4, 0x00, 0x00, 0x00, 0xFF);
		} else if (ShowInfo == 2) {
			float boxX = tlX - 4;
			float boxY = trY + 16;
			float boxScale = ClassWithCameraInfo::instance->screenHeight/250;
			u32 boxLength = -1;
			boxX = drawFloat(ap->owner->pos.x, boxX, boxY, boxScale, boxLength, 2, 0x00, 0x00, 0x00, 0xFF) + 1;
			DrawLine(boxX-0.5*boxScale, boxY-4*boxScale, boxX, boxY-3.25*boxScale, z, 0x00, 0x00, 0x00, 0xFF);
			drawFloat(ap->owner->pos.y, boxX, boxY, boxScale, boxLength, 2, 0x00, 0x00, 0x00, 0xFF);	
		}

		switch (ap->collisionCheckType) {
			case 2: // vert trapezoid
				tlY = centreY + ap->trpValue0;
				trY = centreY + ap->trpValue2;
				blY = centreY + ap->trpValue1;
				brY = centreY + ap->trpValue3;
				break;
			case 3: // horz trapezoid
				tlX = centreX + ap->trpValue0;
				trX = centreX + ap->trpValue1;
				blX = centreX + ap->trpValue2;
				brX = centreX + ap->trpValue3;
				break;
		}

		if (ap->owner->stageActorType != 1) {
			u32 uptr = (u32)ap;
			u8 r = 0xFF;
			u8 g = 0x00;
			u8 b = 0x00;
			u8 a = 0xFF;
		
			if (ap->collisionCheckType == 1) {  //circle
				DrawCircle(centreX, centreY, edgeDistY, edgeDistY, z, 0xFF, 0x00, 0x00, 0xFF);
			} else {
				DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, z, r, g, b, a);
			}
		}
		else 
		{
			//OSReport("Player has : DistToC=%f,%f DistToEdge=%f,%f Pos=%f,%f Scale=%f,%f\n", ap->info.xDistToCenter, ap->info.yDistToCenter, ap->info.xDistToEdge, ap->info.yDistToEdge, ap->owner->pos.x, ap->owner->pos.y, ap->owner->scale.x, ap->owner->scale.y);
			u32 uptr = (u32)ap;
			u8 r = 0xFF;
			u8 g = 0x00;
			u8 b = 0x00;
			u8 a = 0xFF;
			
			DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, z, r, g, b, a);
			
		}

		ap = ap->listPrev;
	}

	// Now, the hardest one.. CollisionMgr_c!
	fBase_c *fb = 0;

	while ((fb = fBase_c::searchByBaseType(2, fb))) {
		u32 uptr = (u32)fb;
		u8 r = 0x00;
		u8 g = 0xFF;
		u8 b = 0xFF;
		u8 a = 0xFF;

		dStageActor_c *ac = (dStageActor_c*)fb;
		if (ac->stageActorType == 1 || ac->stageActorType == 1 || HitboxFormat == 2) {
				
			sensorBase_s *sensors[4] = {
				ac->collMgr.pBelowInfo, ac->collMgr.pAboveInfo,
				ac->collMgr.pAdjacentInfo, ac->collMgr.pAdjacentInfo};

			for (int i = 0; i < 4; i++) {
				sensorBase_s *s = sensors[i];
				if (!s)
					continue;

				float mult = (i == 3) ? -1.0f : 1.0f;

				switch (s->flags & SENSOR_TYPE_MASK) {
					case SENSOR_POINT:
						GXSetPointSize(12, GX_TO_ZERO);					
						DrawPoint(ac->pos.x + (mult * (s->asPoint()->x / 4096.0f)), ac->pos.y + (s->asPoint()->y / 4096.0f), 8005.0f, r, g, b, a);
						break;
					case SENSOR_LINE:
						if (i < 2) {
							DrawLine(
								ac->pos.x + (s->asLine()->lineA / 4096.0f),
								ac->pos.y + (s->asLine()->distanceFromCenter / 4096.0f),
								ac->pos.x + (s->asLine()->lineB / 4096.0f),
								ac->pos.y + (s->asLine()->distanceFromCenter / 4096.0f),
								4096.0f, r, g, b, a);
						} else {
							float rightCorrection = (i == 2) ? -1.0 : 0.0;
							DrawLine(
								ac->pos.x + (mult * (s->asLine()->distanceFromCenter / 4096.0f)) + rightCorrection,
								ac->pos.y + (s->asLine()->lineA / 4096.0f),
								ac->pos.x + (mult * (s->asLine()->distanceFromCenter / 4096.0f)) + rightCorrection,
								ac->pos.y + (s->asLine()->lineB / 4096.0f),
								z, r, g, b, a
							);
						}
						
						break;
				}
			}
			if (ac->stageActorType == 1 && HitboxFormat == 2)  {
				DrawLine(ac->pos.x, ac->pos.y - 2, ac->pos.x, ac->pos.y + 2, z, 0x93,0x70,0xDB,0xFF);
			//}  else if (ac->stageActorType == 1)  {
			//	GXSetPointSize(12, GX_TO_ZERO);	
			//	DrawPoint(ac->pos.x, ac->pos.y, z, 0x93,0x70,0xDB,0xFF);
			}
			/*if (ac->stageActorType == 1) {				
				float boxX = ClassWithCameraInfo::instance->screenLeft+(ClassWithCameraInfo::instance->screenWidth/3.15);
				float boxY = ClassWithCameraInfo::instance->screenTop-(ClassWithCameraInfo::instance->screenHeight/20);
				u8 boxRows = 2;
				u32 boxLength = 24;
				float boxScale = ClassWithCameraInfo::instance->screenHeight/150;
				
				drawInfoBox(boxX, boxY, boxRows, boxLength, boxScale);
				drawFloat(ac->pos_delta.x, boxX+1*boxScale, boxY-1*boxScale, boxScale, boxLength, 4, 0x00, 0x00, 0x00, 0xFF);
				drawFloat(ac->pos.x, boxX+1*boxScale, boxY-7*boxScale, boxScale, boxLength, 2, 0x00, 0x00, 0x00, 0xFF);
				
				boxX = boxX+boxLength*boxScale;
				drawInfoBox(boxX, boxY, boxRows, boxLength, boxScale);
				drawFloat(ac->pos_delta.y, boxX+1*boxScale, boxY-1*boxScale, boxScale, boxLength, 4, 0x00, 0x00, 0x00, 0xFF);
				drawFloat(ac->pos.y, boxX+1*boxScale, boxY-7*boxScale, boxScale, boxLength, 2, 0x00, 0x00, 0x00, 0xFF);
			}*/
		}
		
	}
}

void APDebugDrawer::drawOpa() {
	drawXlu();
}
void APDebugDrawer::drawXlu() {
	GXClearVtxDesc();

	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	GXSetNumIndStages(0);
	for (int i = 0; i < 0x10; i++)
		GXSetTevDirect(i);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor){255,255,255,255});
	GXSetChanMatColor(GX_COLOR0A0, (GXColor){255,255,255,255});
	GXSetNumTexGens(0);

	GXSetNumTevStages(1);
	GXSetNumIndStages(0);

	GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);

	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
//	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
//	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
//	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

	GXSetZCompLoc(GX_FALSE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

	GXSetFog(GX_FOG_NONE, 0, 0, 0, 0, (GXColor){0,0,0,0});
	GXSetFogRangeAdj(GX_FALSE, 0, 0);

	GXSetCullMode(GX_CULL_NONE);

	GXSetDither(GX_TRUE);
	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(18, GX_TO_ZERO);

	GXSetTevColor(GX_TEVREG0, (GXColor){255,255,255,255});
	GXSetTevColor(GX_TEVREG1, (GXColor){0,0,0,255});

	nw4r::g3d::Camera cam(GetCameraByID(GetCurrentCameraID()));
	Mtx matrix;
	cam.GetCameraMtx(&matrix);
	GXLoadPosMtxImm(matrix, 0);
	GXSetCurrentMtx(0);

	// Tileset Display 
	drawTilesetOverlay();
	drawHitboxes();
	drawInputDisplay(
		ClassWithCameraInfo::instance->screenLeft+(ClassWithCameraInfo::instance->screenWidth/7),
		ClassWithCameraInfo::instance->screenTop-(ClassWithCameraInfo::instance->screenHeight/7),
		ClassWithCameraInfo::instance->screenHeight/190
	);
	//drawInputDisplay(ac->pos.x-(screenWidth/13), ac->pos.y+(screenHeight/7), screenHeight/190);
}