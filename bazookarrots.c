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
#include "math.h"
#include "tile.h"

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
#define RABBIT_ANIMATION_FRAMES 4
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Running to target, 2 = Eating, 3 = Hit
	u16 actionTimer; // If not zero, do not change state
    u8 target; // Index in target list
	VectorI16 pos;
} Rabbit;

#define BULLET_SIZE 16
#define BULLET_SIZE_HALF 8
#define BULLET_SPEED 2;
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Moving
    VectorI16 vel; // Direction and speed
	VectorI16 pos;
} Bullet;

#define PLAYER_SIZE 16
#define MAX_CARROTS_IN_CARRY 3
#define PLAYER_SPEED 4;
#define PLAYER_SPEED_DIAGONAL 2;
#define PLAYER_ANIMATION_FRAMES 4
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Moving
	u8 dir; // 0 = Up, clockwise, 8 directions
    VectorI16 vel; // Direction and speed
	VectorI16 pos;
	u8 carrots;
	bool flipHorizontal;
} Player;

// Game data
#define TARGET_COUNT 4
#define RABBIT_COUNT 2
Target targets[TARGET_COUNT];
Rabbit rabbits[RABBIT_COUNT];
Bullet bullets[MAX_CARROTS_IN_CARRY];
Player player;

// Function prototypes
void InitDraw();
void UpdateDraw();

void InitGameData();
void UpdateGame();

void InitGameOver();
void UpdateGameOver();

bool CheckBoxCollision(VectorI16* posA, VectorI16* posB, u8 sizeA, u8 sizeB);
bool InInBounds(VectorI16* pos, u8 size);
void CheckShootInputAndMaybeShoot();
void MoveRabbitToSpawn(u8 index);

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// V9990 4-bits background data
#include "content/tile/data_bg_4b.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16or.h"
#include "gfx/carrot.h"
#include "gfx/carrot_ground.h"
#include "gfx/rabbit.h"
#include "gfx/player.h"

// Fonts
#include "font/font_mgl_sample8.h"

// Character animation
const u8* g_RotAnim[] = { g_PatternData, g_PatternDataRotLeft, g_PatternDataRotHalf, g_PatternDataRotRight };

// states
const FSM_State g_StateGame = {1, InitGameData, UpdateGame, NULL};
const FSM_State g_GameOver = {2, InitGameOver, UpdateGameOver, NULL};

// Sample level tiles map
const u8 g_TileMap[] =
{
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 132, 133, 134, 135, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 164, 165, 166, 167, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 132, 133, 134, 135, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 000, 000, 000, 000, 000, 000, 000, 164, 165, 166, 167, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 168, 169, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 
	 000, 000, 000, 000, 000, 000, 136, 137, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 168, 169, 000, 
	 000, 000, 000, 000, 000, 000, 168, 169, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 168, 169, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,  28,  29, 000, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,  32,  33,  34,  35, 000, 000, 000, 000, 000, 000,  59,  60,  61,  62, 000, 000, 
	 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,  96,  97,  98,  99, 000, 000, 000, 000, 000,  90,  91,  92,  93,  94,  95, 000, 
	 000, 000, 000, 000, 000, 000,  32,  33,  34,  35, 000, 000, 000, 000, 000, 000,  64,  65,  66,  67, 000, 000, 000, 000, 000, 122, 123, 124, 125, 126, 127, 000, 
	 000, 000, 000, 000, 000, 000,  96,  97,  98,  99, 000, 000, 000, 000, 000, 000,  96,  97,  98,  99, 000, 000, 000, 000, 000, 154, 155, 156, 157, 158, 159, 000, 
	 000, 000, 000, 000, 000, 000,  64,  65,  66,  67, 000, 000, 000, 000, 000, 000,  64,  65,  66,  67, 000, 000, 000, 000, 000, 000, 000, 188, 189, 000, 000, 000, 
	  33,  34,  33,  34,  33,  34,   8,  97,  98,  69,  33,  34,  33,  34,  33,  34,   8,  97,  98,  69,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
};

const u8 g_TreeTileMap[] =
{
	 000, 000,  28,  29, 000, 000,
	 000,  59,  60,  61,  62, 000,
	  90,  91,  92,  93,  94,  95,
	 122, 123, 124, 125, 126, 127,
	 154, 155, 156, 157, 158, 159,
	 000, 000, 188, 189, 000, 000,
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 timeLeft = 60;

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data for player
u8 g_PatternData[PATTERN_16_NUM * 8];

u8 g_PatternDataRotRight[PATTERN_16_NUM * 8];
u8 g_PatternDataRotLeft[PATTERN_16_NUM * 8];
u8 g_PatternDataRotHalf[PATTERN_16_NUM * 8];


// Sprite buffer
u8 g_Buffer1[32];
u8 g_Buffer2[32];
u8 g_Buffer3[32];
u8 g_Buffer4[32];

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
	InitDraw();
	for(i = 0; i < TARGET_COUNT; ++i)
	{
		targets[i].type = 0;
		targets[i].pos.x = 10 + 5 * i;
		targets[i].pos.y = 10;
	}

	for(i = 0; i < RABBIT_COUNT; ++i)
	{
		MoveRabbitToSpawn(i);
		rabbits[i].state = 0;
		rabbits[i].actionTimer = i * 60;
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

	if(Keyboard_IsKeyPressed(KEY_UP) && Keyboard_IsKeyPressed(KEY_RIGHT))
	{
		player.pos.y -= PLAYER_SPEED_DIAGONAL;
		player.pos.x += PLAYER_SPEED_DIAGONAL;
		player.dir = 1;
		player.flipHorizontal = false;
	}
	else if(Keyboard_IsKeyPressed(KEY_UP) && Keyboard_IsKeyPressed(KEY_LEFT))
	{
		player.pos.y -= PLAYER_SPEED_DIAGONAL;
		player.pos.x -= PLAYER_SPEED_DIAGONAL;
		player.dir = 7;
		player.flipHorizontal = true;
	}
	else if(Keyboard_IsKeyPressed(KEY_DOWN) && Keyboard_IsKeyPressed(KEY_RIGHT))
	{
		player.pos.y += PLAYER_SPEED_DIAGONAL;
		player.pos.x += PLAYER_SPEED_DIAGONAL;
		player.dir = 3;
		player.flipHorizontal = false;
	}
	else if(Keyboard_IsKeyPressed(KEY_DOWN) && Keyboard_IsKeyPressed(KEY_LEFT))
	{
		player.pos.y += PLAYER_SPEED_DIAGONAL;
		player.pos.x -= PLAYER_SPEED_DIAGONAL;
		player.dir = 5;
		player.flipHorizontal = true;
	}
	else if(Keyboard_IsKeyPressed(KEY_UP))
	{
		player.pos.y -= PLAYER_SPEED;
		player.dir = 0;
	}
	else if(Keyboard_IsKeyPressed(KEY_DOWN))
	{
		player.pos.y += PLAYER_SPEED;
		player.dir = 4;
	}
	else if(Keyboard_IsKeyPressed(KEY_RIGHT))
	{
		player.pos.x += PLAYER_SPEED;
		player.flipHorizontal = false;
		player.dir = 2;
	}
	else if(Keyboard_IsKeyPressed(KEY_LEFT))
	{
		player.dir = 6;
		player.pos.x -= PLAYER_SPEED;
		player.flipHorizontal = true;
	}
	else
	{
		// TODO: Stop player walk
	}
	CheckShootInputAndMaybeShoot();

	// Clamp player to play area
	player.pos.x = Clamp16(player.pos.x, 0, TILE_SCREEN_WIDTH - PLAYER_SIZE);
	player.pos.y = Clamp16(player.pos.y, 32, TILE_SCREEN_HEIGHT - 40);

	// Update bullets
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
				if(rabbits[j].state != 0)
				{
					if(CheckBoxCollision(&bullets[i].pos, &rabbits[j].pos, BULLET_SIZE, RABBIT_SIZE))
					{
						// Rabbit is hit with a bullet!
						bullets[i].state == 0;
						rabbits[i].state = 3;
						rabbits[i].actionTimer = i * 120;
					}
				}
			}

			// Are we out of bounds?
			if(!InInBounds(&bullets[i].pos, BULLET_SIZE_HALF))
			{
				bullets[i].state = 0;
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
				rabbits[i].target = Math_GetRandomRange8(0, TARGET_COUNT);
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
				if(tempY > rabbits[i].pos.y)
				{
					rabbits[i].pos.y += RABBIT_SPEED;
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
			else if(rabbits[i].state == 2) // Eating, get new target
			{
				rabbits[i].state == 0;
			}
			else if(rabbits[i].state == 3) // Dead rabbit, respawn
			{
				MoveRabbitToSpawn(i);
				rabbits[i].state == 0;
			}
		}
		else
		{
			// Decrease action timer, when it hits zero rabbit will continue update again
			rabbits[i].actionTimer--;
		}
	}
	UpdateDraw();

	if (g_Frame % 60 == 0)
		timeLeft -= 1;

	if (timeLeft == 0)
		FSM_SetState(&g_GameOver);
}

void MoveRabbitToSpawn(u8 index)
{
	rabbits[index].state = 0;
	rabbits[index].pos.x = Math_GetRandomRange8(10, 240);
	rabbits[index].pos.y = 200;
}

// Pos is top left corner
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
	  (pos->x > -size
	&& pos->x < (256 + size)
	&& pos->y < (211 + size)
	&& pos->y > -size);
}

void CheckShootInputAndMaybeShoot()
{
	if(Keyboard_IsKeyPushed(KEY_SPACE))
	{
		for(i = 0; i < MAX_CARROTS_IN_CARRY; ++i)
		{
			if(bullets[i].state == 0)
			{
				bullets[i].pos.x = player.pos.x;
				bullets[i].pos.y = player.pos.y;
				bullets[i].state = 1;
				if(player.dir == 0)
				{
					bullets[i].vel.x = 0;
					bullets[i].vel.y = -BULLET_SPEED;
				}
				else if(player.dir == 1)
				{
					bullets[i].vel.x = BULLET_SPEED;
					bullets[i].vel.y = -BULLET_SPEED;
				}
				else if(player.dir == 2)
				{
					bullets[i].vel.x = BULLET_SPEED;
					bullets[i].vel.y = 0;
				}
				else if(player.dir == 3)
				{
					bullets[i].vel.x = BULLET_SPEED;
					bullets[i].vel.y = BULLET_SPEED;
				}
				else if(player.dir == 4)
				{
					bullets[i].vel.x = 0;
					bullets[i].vel.y = BULLET_SPEED;
				}
				else if(player.dir == 5)
				{
					bullets[i].vel.x = -BULLET_SPEED;
					bullets[i].vel.y = BULLET_SPEED;
				}
				else if(player.dir == 6)
				{
					bullets[i].vel.x = -BULLET_SPEED;
					bullets[i].vel.y = 0;
				}
				else if(player.dir == 7)
				{
					bullets[i].vel.x = -BULLET_SPEED;
					bullets[i].vel.y = -BULLET_SPEED;
				}

				break;
			}
		}
	}
}

// Game over

void InitGameOver()
{
	// VDP_SetMode(VDP_MODE_SCREEN4);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);

	Print_DrawTextAt(10, 10, "GAME OVER");	
}

void UpdateGameOver()
{
	
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
void InitDraw()
{
	VDP_ClearVRAM();

	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_SetPage(0);

	// Setup tilemap
	Tile_SetBankPage(2);
	Tile_FillBank(0, 6);
	Tile_FillBank(1, 7);
	Tile_FillBank(2, 8);
	Tile_FillBank(3, 9);
	Tile_LoadBank(0, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);
	Tile_LoadBank(2, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);
	//for(u8 i = 0; i < 15; ++i)
	//	VDP_SetPaletteEntry(i + 1, *(u16*)&g_DataBG4b_palette[i*2]);

	// Draw level
	Tile_SetDrawPage(0);
	Tile_SelectBank(0);
	Tile_FillScreen(6);
	Tile_DrawMapChunk( 0, 15, g_TreeTileMap, 6, 6); // Draw tree tilemap
	Tile_DrawMapChunk( 3, 15, g_TreeTileMap, 6, 6);
	Tile_DrawMapChunk(11, 15, g_TreeTileMap, 6, 6);
	Tile_DrawScreen(g_TileMap); // Draw the whole screen tilemap
	Tile_DrawBlock(10, 8, 4, 4, 4, 2); // Draw a cloud (4x2 tiles)

	// Setup and draw UI
	Print_SetBitmapFont(g_Font_MGL_Sample8);
	Print_SetColor(0x11, 0x66);
	Print_SetPosition(4, 4);
	Print_DrawText("TIME");
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

	// Load 16x16 sprites (Pattern 96~143)
	u8* ptr = g_PatternData;
	// loops thru 8x8 sprites
	for(u8 i5 = 0; i5 < 6; i5++) // white sprites 
	{
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  0) * 8, ptr, 8); ptr += 8; // left up
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 12) * 8, ptr, 8); ptr += 8; // left down
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  1) * 8, ptr, 8); ptr += 8; // right up
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 13) * 8, ptr, 8); ptr += 8; // right down
	}
	for(u8 i6 = 0; i6 < 6; i6++) // black sprites
	{
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 24) * 8, ptr, 8); ptr += 8; // left up
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 36) * 8, ptr, 8); ptr += 8; // left down
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 25) * 8, ptr, 8); ptr += 8; // right up
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 37) * 8, ptr, 8); ptr += 8; // right down
	}

	// Initialize 16x16 OR sprites
	VDP_SetPaletteEntry(2, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(3, RGB16(6, 4, 1));
	
	// player
	VDP_SetSpriteExUniColor(0, 0, 0, 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 0, 1 * 4, VDP_SPRITE_CC + 0x01);

	// rotating bersons
	VDP_SetSpriteExUniColor(2, 0, 0, 2 * 4, 0x02);
	VDP_SetSpriteExUniColor(3, 0, 0, 3 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(4, 0, 0, 4 * 4, 0x02);
	VDP_SetSpriteExUniColor(5, 0, 0, 5 * 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(6, 0, 0, 6 * 4, 0x02);
	VDP_SetSpriteExUniColor(7, 0, 0, 7 * 4, VDP_SPRITE_CC + 0x01);

	// bullets
	const int carrot_index = 8;
	for (i = 0; i < MAX_CARROTS_IN_CARRY; ++i )
	{
		VDP_SetSpriteExUniColor(carrot_index + i,     0, 0, (carrot_index + i) * 4, 0x03);
		VDP_SetSpriteExUniColor(carrot_index + 1 + i, 0, 0, (carrot_index + 1 + i) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_carrot,                     (carrot_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_carrot + 4 * 8,             (carrot_index + 1 + i) * 4, 4);
	}

	// rabbits
	const int rabbit_index = carrot_index + MAX_CARROTS_IN_CARRY * 2;
	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		VDP_SetSpriteExUniColor(rabbit_index + i,     0, 0,               (rabbit_index + i) * 4, 0x03);
		VDP_SetSpriteExUniColor(rabbit_index + 1 + i, 0, 0,               (rabbit_index + 1 + i) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_rabbit,                                   (rabbit_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_rabbit + 4 * RABBIT_ANIMATION_FRAMES * 8, (rabbit_index + 1 + i) * 4, 4);
	}

	VDP_HideSpriteFrom(8);

	// Compute transformed sprite data
	loop(i, 6 * 2)
	{
		u16 idx = i * 4 * 8;
		SpriteFX_RotateRight16(&g_PatternData[idx], &g_PatternDataRotRight[idx]);
		SpriteFX_RotateLeft16(&g_PatternData[idx], &g_PatternDataRotLeft[idx]);
		SpriteFX_RotateHalfTurn16(&g_PatternData[idx], &g_PatternDataRotHalf[idx]);
	}

	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (0*256*8), 32*4*8);
	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (1*256*8), 32*4*8);
	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (2*256*8), 32*4*8);
}

//-----------------------------------------------------------------------------
void UpdateDraw()
{
	// DRAW PLAYER

	u8 frame = (g_Frame >> 2) % 6;
	u8 pat = (frame * 8 * 4);
	u8* pat1 = g_PatternData + pat;
	u8* pat2 = g_PatternData + pat + 24 * 8;

	// Flip horizontal
	VDP_SetSpritePosition(0, player.pos.x, player.pos.y); // white sprite
	VDP_SetSpritePosition(1, player.pos.x, player.pos.y); // black sprite
	if(player.flipHorizontal)
	{
		SpriteFX_FlipHorizontal16(pat1, g_Buffer1);
		SpriteFX_FlipHorizontal16(pat2, g_Buffer2);
		VDP_LoadSpritePattern(g_Buffer1, 0, 4);
		VDP_LoadSpritePattern(g_Buffer2, 4, 4);
	}
	else {
		VDP_LoadSpritePattern(pat1, 0, 4);
		VDP_LoadSpritePattern(pat2, 4, 4);
	}

	// // Rotate 90°
	// u8 rot = (g_Frame >> 4) % 4;
	// VDP_LoadSpritePattern(g_RotAnim[rot] + pat, 24, 4);
	// VDP_LoadSpritePattern(g_RotAnim[rot] + pat + (24 * 8), 28, 4);

	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		VDP_SetSpritePosition(14 + i,     rabbits[i].pos.x, rabbits[i].pos.y);
		VDP_SetSpritePosition(14 + 1 + i, rabbits[i].pos.x, rabbits[i].pos.y);
	}

	// DRAW CARROTS
	for(i = 0; i < MAX_CARROTS_IN_CARRY; ++i)
	{

		if(bullets[i].state == 1)
		{
			// VDP_SetSprite(8 + i, bullets[i].pos.x, bullets[i].pos.y, (4 * 8) + i * 4);
			// VDP_SetSprite(9 + i, bullets[i].pos.x, bullets[i].pos.y, (4 * 9) + i * 4);
			VDP_SetSpritePosition(8 + i, bullets[i].pos.x, bullets[i].pos.y);
			VDP_SetSpritePosition(9 + i, bullets[i].pos.x, bullets[i].pos.y);
			
			// u8 rot = (g_Frame >> 4) % 4;
			//VDP_SetSpritePosition(2 + i, bullets[i].pos.x, bullets[i].pos.y);
			//VDP_SetSpritePosition(3 + i, bullets[i].pos.x, bullets[i].pos.y);
			// VDP_LoadSpritePattern(g_RotAnim[rot] + pat, (2 + i) * 4, 4);
			// VDP_LoadSpritePattern(g_RotAnim[rot] + pat + (24 * 8), (3 + i) * 4, 4);
			//VDP_LoadSpritePattern(pat1, 8 + i * 4, 4);
			//VDP_LoadSpritePattern(pat2, 12 + i * 4, 4);
		}
		else
		{
			//VDP_HideSprite(8 + i);
			//VDP_HideSprite(9 + i);
		}
	}

	// Draw UI
	Print_SetPosition(40, 4);
	Print_DrawInt(timeLeft);

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
	VDP_EnableVBlank(TRUE);

	FSM_SetState(&g_StateGame);

	bool bContinue = TRUE;
	while(bContinue)
	{
		Keyboard_Update();
		// VDP_SetColor(COLOR_DARK_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_LIGHT_BLUE);

		FSM_Update();

		// Print_DrawCharAt(31, 0, g_CharAnim[g_Frame & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
		
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


