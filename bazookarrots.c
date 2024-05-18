// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Sprite tool sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "bios.h"
#include "memory.h"
#include "fsm.h"
#include "sprite_fx.h"

//=============================================================================
// DEFINES
//=============================================================================

// Pattern enums
#define PATTERN_16_NUM		(u8)(6*2*4)

// Functions
typedef void (*fxFunc)(const u8*, u8*);

// 8-bits unsigned vector structure
typedef struct
{
	const u8* Name;
	fxFunc    Func;
} EffectInfo;

// Game structs
#define TARGET_SIZE 16
typedef struct
{
    u8 type; // 0 = Nothing, 1 = Carrot
	VectorI16 pos;
} Target;

#define RABBIT_SIZE 16
#define RABBIT_SPEED 1
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Running to target, 2 = Eating, 3 = Hit
	u8 actionTimer; // If not zero, do not change state
    u8 target; // Index in target list
	VectorI16 pos;
} Rabbit;

#define BULLET_SIZE 16
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Moving
    VectorI16 vel; // Direction and speed
	VectorI16 pos;
} Bullet;

#define PLAYER_SIZE 16
#define MAX_CARROTS_IN_CARRY 3
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Moving
    VectorI16 vel; // Direction and speed
	VectorI16 pos;
	u8 carrots;
} Player;

// Game data
#define TARGET_COUNT 4
#define RABBIT_COUNT 2
Target targets[TARGET_COUNT];
Rabbit rabbits[RABBIT_COUNT];
Bullet bullets[MAX_CARROTS_IN_CARRY];
Player player;

// Function prototypes
void Init16();
void Update16();

void InitGameData();
void UpdateGame();
bool CheckBoxCollision(VectorI16* posA, VectorI16* posB, u8 sizeA, u8 sizeB);
bool InInBounds(VectorI16* pos, u8 size);


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"
#include "font/font_mgl_symbol1.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16or.h"

// Character animation
const u8 g_CharAnim[] = { '|', '\\', '-', '/' };
//
const u8* g_RotAnim[] = { g_PatternData, g_PatternDataRotLeft, g_PatternDataRotHalf, g_PatternDataRotRight };

// states
const FSM_State g_State16 = { 0, Init16, Update16, NULL };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data
u8 g_PatternData[PATTERN_16_NUM * 8];
u8 g_PatternDataRotRight[PATTERN_16_NUM * 8];
u8 g_PatternDataRotLeft[PATTERN_16_NUM * 8];
u8 g_PatternDataRotHalf[PATTERN_16_NUM * 8];

// Sprite buffer
u8 g_Buffer1[32];
u8 g_Buffer2[32];
u8 g_Buffer3[32];
u8 g_Buffer4[32];

// Sprite X position
u8 g_PosX0;
u8 g_PosX1;

//
u8 g_FXIndex;

u8 i;
u8 j;
u8 k;
u8 tempX;
u8 tempY;

//=============================================================================
// GAME FUNCTIONS
//=============================================================================

void InitGameData()
{
	for(i = 0; i < TARGET_COUNT; ++i)
	{
		targets[i].type = 0;
		targets[i].pos.x = 10 + 5 * i;
		targets[i].pos.y = 10;
	}

	for(i = 0; i < RABBIT_COUNT; ++i)
	{
		targets[i].type = 0;
		targets[i].pos.x = 0;
		targets[i].pos.y = 200;
	}

	for(i = 0; i < MAX_CARROTS_IN_CARRY; ++i)
	{
		bullets[i].state = 0;
		bullets[i].pos.x = 0;
		bullets[i].pos.y = 0;
		bullets[i].vel.x = 0;
		bullets[i].vel.y = 0;
	}

	player.state = 1;
	player.pos.x = 20;
	player.pos.y = 40;
	player.vel.x = 0;
	player.vel.y = 0;
}

void UpdateGame()
{
	// Update rabbits
	// Update player
	// Update bullets
	// Update UI

	for(i = 0; i < MAX_CARROTS_IN_CARRY; ++i)
	{
		if(bullets[i].state == 1)
		{
			// Move bullet
			bullets[i].pos.x += bullets[i].vel.x;
			bullets[i].pos.y += bullets[i].vel.y;

			// Did we hit a rabbit?
			for(j = 0; j < RABBIT_COUNT; ++j)
			{
				if(CheckBoxCollision(&bullets[i].pos, &rabbits[j].pos, BULLET_SIZE, RABBIT_SIZE))
				{
					bullets[i].state == 0;
					// Rabbit is hit with a bullet!
				}
			}

			// Are we out of bounds?
			if(!InInBounds(&bullets[i].pos, BULLET_SIZE))
			{
				bullets[i].state == 0;
				// Out of bounds!
			}
		}
	}

	for(i = 0; i < RABBIT_COUNT; ++i)
	{
		if(rabbits[i].actionTimer == 0)
		{
			if(rabbits[i].state == 0) // Active inactive rabbit
			{
				rabbits[i].state = 1;
				rabbits[i].target = 0; // TODO: Make this random from targets
				// TODO: Play walk animation
			}
			else if(rabbits[i].state == 1) // Run towards current target
			{
				// Move rabbit
				tempX = targets[rabbits[i].target].pos.x;
				tempY = targets[rabbits[i].target].pos.y;
				if(tempX > rabbits[i].pos.x)
				{
					rabbits[i].pos.x += RABBIT_SPEED;
				}
				else if(tempX < rabbits[i].pos.x)
				{
					rabbits[i].pos.x -= RABBIT_SPEED;
				}
				else if(tempY < rabbits[i].pos.y)
				{
					rabbits[i].pos.y -= RABBIT_SPEED;
				}
				else if(tempY < rabbits[i].pos.y)
				{
					rabbits[i].pos.y -= RABBIT_SPEED;
				}

				// Are we at the target?
				if(CheckBoxCollision(&rabbits[i].pos, &targets[rabbits[i].target].pos, RABBIT_SIZE, TARGET_SIZE))
				{
					if(targets[rabbits[i].target].type == 1)
					{
						rabbits[i].state == 2; // Eat target
						rabbits[i].actionTimer = 60;
						// TODO: Play eat sound
						// TODO: Play eat animation
					}
					else
					{
						rabbits[i].state == 0; // Get a new target
						rabbits[i].actionTimer = 30;
					}
				}
			}
			else if(rabbits[i].state == 2) // Eating, do nothing
			{

			}
		}
		else
		{
			// Decrease action timer, when it hits zero rabbit will continue update again
			rabbits[i].actionTimer--;
		}
	}
}

// Pos is top right corner
// Return true if collision occures
bool CheckBoxCollision(VectorI16* posA, VectorI16* posB, u8 sizeA, u8 sizeB)
{
	return
	  (posA->x < posB->x + sizeB
	&& posA->x + sizeA > posB->x
	&& posA->y < posB->y + sizeB
	&& posA->y + sizeA > posB->y);
}

// Is the position on the screen
// TODO: Maybe take size into account here?
bool InInBounds(VectorI16* pos, u8 size)
{
	return
	  (pos->x > 0 - size
	&& pos->x < 256
	&& pos->y < 211
	&& pos->y > 0 - size);
}

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//-----------------------------------------------------------------------------
void Init16()
{
	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN4);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_ClearVRAM();
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

	// Load 16x16 sprites (Pattern 96~143)
	u8* ptr = g_PatternData;
	for(u8 i5 = 0; i5 < 6; i5++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  0) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 12) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  1) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 13) * 8, ptr, 8); ptr += 8;
	}
	for(u8 i6 = 0; i6 < 6; i6++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 24) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 36) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 25) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 37) * 8, ptr, 8); ptr += 8;
	}

	// Initialize 16x16 OR sprites
	VDP_SetPaletteEntry(2, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(3, RGB16(6, 4, 1));
	VDP_SetSpriteExUniColor(0, 0, 32-1, 0, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 32-1, 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(2, 0, 80-1, 8, 0x02);
	VDP_SetSpriteExUniColor(3, 0, 80-1, 12, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(4, 0, 128-1, 16, 0x02);
	VDP_SetSpriteExUniColor(5, 0, 128-1, 20, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(6, 64, (u8)(176-1), 24, 0x02);
	VDP_SetSpriteExUniColor(7, 64, (u8)(176-1), 28, VDP_SPRITE_CC + 0x01);
	VDP_HideSpriteFrom(8);
	g_PosX0 = 0;
	g_PosX1 = 0;

	// Compute transformed sprite data
	loop(i, 6 * 2)
	{
		u16 idx = i * 4 * 8;
		SpriteFX_RotateRight16(&g_PatternData[idx], &g_PatternDataRotRight[idx]);
		SpriteFX_RotateLeft16(&g_PatternData[idx], &g_PatternDataRotLeft[idx]);
		SpriteFX_RotateHalfTurn16(&g_PatternData[idx], &g_PatternDataRotHalf[idx]);
	}

	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(0xF, 0x4);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (0*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (1*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (2*256*8), 32*4*8);

	Print_DrawTextAt(0, 0, "\x2\x3\x4\x5 Sprite FX sample (16x16)");
	Print_DrawCharXAt(0, 1, '\x17', 32);

	Print_DrawTextAt(1, 4, "Crop R/L");
	Print_DrawTextAt(15, 3, "\x9F\x9F");
	Print_DrawTextAt(15, 4, "\x9F\x9F");
	Print_DrawTextAt(15, 5, "\x9F\x9F");
	Print_DrawCharXAt(0, 6, '\x9F', 32);

	Print_DrawTextAt(1, 10, "Flip H");
	Print_DrawCharXAt(0, 12, '\x9F', 32);

	Print_DrawTextAt(1, 22, "Rotate");

	Print_DrawTextAt(31-7, 22, "\x82:Flip V");
	Print_DrawTextAt(31-7, 23, "\x83:8x8");
}

//-----------------------------------------------------------------------------
void Update16()
{
	bool bToggle = Keyboard_IsKeyPressed(KEY_UP) || Keyboard_IsKeyPressed(KEY_DOWN);

	u8 frame = (g_Frame >> 2) % 6;
	u8 pat = (frame * 8 * 4);

	// Crop right/left
	g_PosX0++;
	VDP_SetSpritePositionX(0, g_PosX0);
	VDP_SetSpritePositionX(1, g_PosX0);
	u8* pat1 = g_PatternData + pat;
	u8* pat2 = g_PatternData + pat + 24 * 8;
	if((g_PosX0 > 104) && (g_PosX0 <= 120))
	{
		u8 offset = g_PosX0 - 105;
		SpriteFX_CropRight16(pat1, g_Buffer1, offset);
		SpriteFX_CropRight16(pat2, g_Buffer2, offset);
		pat1 = g_Buffer1;
		pat2 = g_Buffer2;
	}
	else if((g_PosX0 >= 120) && (g_PosX0 < 136))
	{
		u8 offset = 15 - (g_PosX0 - 120);
		SpriteFX_CropLeft16(pat1, g_Buffer1, offset);
		SpriteFX_CropLeft16(pat2, g_Buffer2, offset);
		pat1 = g_Buffer1;
		pat2 = g_Buffer2;
	}
	if(bToggle)
	{
		SpriteFX_FlipVertical16(pat1, g_Buffer3);
		SpriteFX_FlipVertical16(pat2, g_Buffer4);
		pat1 = g_Buffer3;
		pat2 = g_Buffer4;
	}
	VDP_LoadSpritePattern(pat1, 0, 4);
	VDP_LoadSpritePattern(pat2, 4, 4);

	// Flip horizontal
	g_PosX1--;
	VDP_SetSpritePositionX(2, g_PosX1);
	VDP_SetSpritePositionX(3, g_PosX1);
	pat1 = g_PatternData + pat;
	pat2 = g_PatternData + pat + 24 * 8;
	if(bToggle)
	{
		SpriteFX_Rotate180_16(pat1, g_Buffer1);
		SpriteFX_Rotate180_16(pat2, g_Buffer2);
	}
	else
	{
		SpriteFX_FlipHorizontal16(pat1, g_Buffer1);
		SpriteFX_FlipHorizontal16(pat2, g_Buffer2);
	}
	VDP_LoadSpritePattern(g_Buffer1, 8, 4);
	VDP_LoadSpritePattern(g_Buffer2, 12, 4);

	// Rotate 90°
	u8 rot = (g_Frame >> 4) % 4;
	VDP_LoadSpritePattern(g_RotAnim[rot] + pat, 24, 4);
	VDP_LoadSpritePattern(g_RotAnim[rot] + pat + (24 * 8), 28, 4);

	// if(Keyboard_IsKeyPressed(KEY_SPACE))
	// 	FSM_SetState(&g_State8);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Setup system
	Bios_SetKeyClick(FALSE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	FSM_SetState(&g_State16);

	bool bContinue = TRUE;
	while(bContinue)
	{
		// VDP_SetColor(COLOR_DARK_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_LIGHT_BLUE);

		FSM_Update();

		// Sign-of-life	
		Print_DrawCharAt(31, 0, g_CharAnim[g_Frame & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


