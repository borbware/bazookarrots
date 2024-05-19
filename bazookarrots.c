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
#include "clock.h"

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
	u16 growTimer;
	VectorU8 tilePos;
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
	bool flipHorizontal;
} Rabbit;

#define BULLET_SIZE 16
#define BULLET_SIZE_HALF 8
#define BULLET_SPEED 5;
typedef struct 
{
	u8 state; // 0 = Inactive, 1 = Moving
    VectorI16 vel; // Direction and speed
	VectorI16 pos;
} Bullet;

#define PLAYER_SIZE 16
#define MAX_CARROTS_IN_CARRY 3
#define PLAYER_SPEED 3
#define PLAYER_SPEED_DIAGONAL 2
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
#define TARGET_COUNT 32
#define RABBIT_COUNT 2
Target targets[TARGET_COUNT];
Rabbit rabbits[RABBIT_COUNT];
Bullet bullets[MAX_CARROTS_IN_CARRY];
Player player;

u16 score;

// Function prototypes
void InitDraw();
void UpdateDraw();

void InitGameData();
void UpdateGame();

void InitGameOver();
void UpdateGameOver();

void RearrangeSprites(u8* origData, u8* dest, u8 anim_len);

bool CheckBoxCollision(VectorI16* posA, VectorI16* posB, u8 sizeA, u8 sizeB);
bool InInBounds(VectorI16* pos, u8 size);
void CheckShootInputAndMaybeShoot();
void MoveRabbitToSpawn(u8 index);
u16 GetCarrotGrowTime();
void UpdateTimeUI();
void UpdateScoreUI();
void UpdateCarrotUI();


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// V9990 4-bits background data
#include "content/tile/data_bg_4b.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "gfx/carrot.h"
#include "gfx/carrot_ground.h"
#include "gfx/rabbit.h"
#include "gfx/rabbit_dies.h"
#include "gfx/rabbit_eats2.h"
#include "gfx/player.h"

// Fonts
#include "font/font_mgl_sample8.h"

// Character animation
const u8* g_CarRotAnim[] = { g_CarrotSpriteData, g_CarrotSpriteDataRotLeft, g_CarrotSpriteDataRotHalf, g_CarrotSpriteDataRotRight };

// states
const FSM_State g_StateGame = {1, InitGameData, UpdateGame, NULL};
const FSM_State g_GameOver = {2, InitGameOver, UpdateGameOver, NULL};

// Sample level tiles map
const u8 g_TileMap[] =
{ 
	 000, 000,  28,  29, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	 000,  59,  60,  61,  62, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 168, 169, 000, 000, 000, 132, 133, 134, 135, 000, 000, 000, 000, 000, 000, 000, 
	  90,  91,  92,  93,  94,  95, 000, 000, 132, 133, 134, 135, 000, 000, 000, 000, 000, 000, 000, 000, 000, 164, 165, 166, 167, 000, 000, 000, 000, 000, 000, 000, 
	 122, 123, 124, 125, 126, 127, 000, 000, 164, 165, 166, 167, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 
	 154, 155, 156, 157, 158, 159, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 168, 169, 000, 
	 000, 000, 188, 189, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 136, 137, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 
	  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34,  33,  34, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
	  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97,  98,  97, 
	  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65,  66,  65, 
};

const u8 g_CarrotMap[] =
{
	 156, 157,
	 188, 189,
};

const u8 g_CarrotMapEmpty[] =
{
	 98,  97,
	 188, 189,
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u16 timeLeft = 60;

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data for player

u8 g_PlayerSpriteData[PLAYER_ANIMATION_FRAMES * 2 * 4 * 8];

u8 g_CarrotSpriteData[1 * 2 * 4 * 8];
u8 g_CarrotSpriteDataRotRight[1 * 2 * 4 * 8];
u8 g_CarrotSpriteDataRotLeft[1 * 2 * 4 * 8];
u8 g_CarrotSpriteDataRotHalf[1 * 2 * 4 * 8];

u8 g_RabbitSpriteData[RABBIT_ANIMATION_FRAMES * 2 * 4 * 8];
u8 g_RabbitDiesSpriteData[RABBIT_ANIMATION_FRAMES * 2 * 4 * 8];
u8 g_RabbitEatsSpriteData[RABBIT_ANIMATION_FRAMES * 2 * 4 * 8];

// Sprite buffer
u8 g_Buffer1[32];
u8 g_Buffer2[32];
u8 g_Buffer3[32];
u8 g_Buffer4[32];

u8 g_FXIndex;

u8 i;
u8 j;
u8 k;
i16 tempX;
i16 tempY;

//=============================================================================
// GAME FUNCTIONS
//=============================================================================

void InitGameData()
{
	Math_SetRandomSeed8(RTC_Read(0));
	Math_SetRandomSeed16(RTC_Read(0));

	InitDraw();
	for(i = 0; i < TARGET_COUNT; ++i)
	{
		if(i < 8)
		{
			targets[i].tilePos.x = 4 + i * 3;
			targets[i].tilePos.y = 9;
		}
		else if(i < 16)
		{
			targets[i].tilePos.x = 5 + (i - 8) * 3;
			targets[i].tilePos.y = 13;	
		}
		else if(i < 24)
		{
			targets[i].tilePos.x = 4 + (i - 16) * 3;
			targets[i].tilePos.y = 16;	
		}
		else
		{
			targets[i].tilePos.x = 5 + (i - 24) * 3;
			targets[i].tilePos.y = 19;	
		}

		if(Math_GetRandomMax16(10) > 4)
		{
			targets[i].type = 1;
			targets[i].growTimer = 0;
			targets[i].tilePos.x += Math_GetRandomMax16(2);
		}
		else
		{
			targets[i].type = 0;
			targets[i].growTimer = GetCarrotGrowTime();
			targets[i].tilePos.y += Math_GetRandomMax16(3) - 1;
		}

		targets[i].pos.x = 8 * targets[i].tilePos.x;
		targets[i].pos.y = 8 * targets[i].tilePos.y;

		if(targets[i].type == 0)
		{
			Tile_DrawMapChunk(targets[i].tilePos.x, targets[i].tilePos.y, g_CarrotMapEmpty, 2, 2);
		}
		else
		{
			Tile_DrawMapChunk(targets[i].tilePos.x, targets[i].tilePos.y, g_CarrotMap, 2, 2);
		}
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

	score = 0;
	timeLeft = 60;
	UpdateTimeUI();
	UpdateScoreUI();
	UpdateCarrotUI();
}

void UpdateGame()
{
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

	if(player.carrots > 0)
	{
		CheckShootInputAndMaybeShoot();
	}

	// Clamp player to play area
	player.pos.x = Clamp16(player.pos.x, 0, TILE_SCREEN_WIDTH - PLAYER_SIZE);
	player.pos.y = Clamp16(player.pos.y, 38, TILE_SCREEN_HEIGHT - PLAYER_SIZE - 8);

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
				if(rabbits[j].state != 0 || rabbits[j].state != 3)
				{
					if(CheckBoxCollision(&bullets[i].pos, &rabbits[j].pos, BULLET_SIZE, RABBIT_SIZE))
					{
						// Rabbit is hit with a bullet!
						bullets[i].state = 0;
						rabbits[j].state = 3;
						rabbits[j].actionTimer = 1 * 60;
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
		if(rabbits[i].actionTimer > 0)
		{
			// Decrease action timer, when it hits zero rabbit will continue update again
			rabbits[i].actionTimer--;
		}

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
					rabbits[i].flipHorizontal = false;
				}
				else if(tempX < rabbits[i].pos.x)
				{
					rabbits[i].pos.x -= RABBIT_SPEED;
					rabbits[i].flipHorizontal = true;
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
					// If target is carrot eat it, if not wait (in both cases, get a new target after anim/wait)
					if(targets[rabbits[i].target].type == 1)
					{
						rabbits[i].state == 2; // Eat target
						rabbits[i].actionTimer = 60;
						targets[rabbits[i].target].type = 0;
						targets[rabbits[i].target].growTimer = GetCarrotGrowTime();
						Tile_DrawMapChunk(targets[rabbits[i].target].tilePos.x, targets[rabbits[i].target].tilePos.y, g_CarrotMapEmpty, 2, 2);
						// TODO: Play eat sound
						// TODO: Play eat animation
					}
					else
					{
						rabbits[i].state == 0; // Get a new target
						rabbits[i].actionTimer = 30;
					}
				}

				if(targets[rabbits[i].target].type == 0) // Get new target if it is eaten
				{
					rabbits[i].target = Math_GetRandomRange8(0, TARGET_COUNT);
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
	}

	// Update carrots
	for(int i = 0; i < TARGET_COUNT; ++i)
	{
		if(targets[i].type == 0) // Grow carrots
		{
			targets[i].growTimer--;
			if(targets[i].growTimer == 0)
			{
				targets[i].type = 1;
				Tile_DrawMapChunk(targets[i].tilePos.x, targets[i].tilePos.y, g_CarrotMap, 2, 2);	
			}
		}
		else // Pick up carrots
		{
			if(player.carrots < MAX_CARROTS_IN_CARRY)
			{
				if(CheckBoxCollision(&player.pos, &targets[i].pos, PLAYER_SIZE, TARGET_SIZE))
				{
					player.carrots++;
					UpdateCarrotUI();
					targets[i].type = 0;
					targets[i].growTimer = GetCarrotGrowTime();
					Tile_DrawMapChunk(targets[i].tilePos.x, targets[i].tilePos.y, g_CarrotMapEmpty, 2, 2);	
				}
			}
		}
	}

	// Return carrots
	if(player.pos.x < 32 && player.pos.y < 42)
	{
		if(player.carrots > 0)
		{
			score += player.carrots;
			player.carrots = 0;
			UpdateCarrotUI();
			UpdateScoreUI();
		}
	}

	UpdateDraw();

	if (g_Frame % 60 == 0)
	{
		timeLeft -= 1;
		UpdateTimeUI();
	}

	if (timeLeft == 0)
		FSM_SetState(&g_GameOver);
}

u16 GetCarrotGrowTime()
{
	return Math_GetRandomRange16(60 * 16, 60 * 20); // Grow new carrot after random time
}

void MoveRabbitToSpawn(u8 index)
{
	rabbits[index].state = 0;
	rabbits[index].pos.x = Math_GetRandomRange16(10, 240);
	rabbits[index].pos.y = TILE_SCREEN_HEIGHT;
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
				player.carrots--;
				UpdateCarrotUI();

				break;
			}
		}
	}
}

// Game over

void InitGameOver()
{
	VDP_HideAllSprites();
	for(u8 i = 0; i < 16; ++i)
		VDP_SetPaletteEntry(i, RGB16(0, 0, 0));
	VDP_SetPaletteEntry(COLOR_WHITE, RGB16(7, 7, 7));
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	Print_SetPosition(80, 80);
	Print_DrawText("GAME OVER");
	Print_SetPosition(80, 90);
	Print_DrawText("SCORE");
	Print_SetPosition(80, 100);
	Print_DrawInt(score);
}

void UpdateGameOver()
{
	if(Keyboard_IsKeyPushed(KEY_SPACE))
	{
		FSM_SetState(&g_StateGame);
	}
}

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

void RearrangeSprites(u8* origData, u8* dest, u8 anim_len)
{
	u8 offset = 0;
	for(u8 i5 = 0; i5 < anim_len; i5++) // white sprites 
	{
		Mem_Copy(origData + (i5 * 2                + offset    ) * 8, dest, 8); dest += 8; // left up
		Mem_Copy(origData + (i5 * 2 + anim_len * 2 + offset    ) * 8, dest, 8); dest += 8; // left down
		Mem_Copy(origData + (i5 * 2                + offset + 1) * 8, dest, 8); dest += 8; // right up
		Mem_Copy(origData + (i5 * 2 + anim_len * 2 + offset + 1) * 8, dest, 8); dest += 8; // right down
	}
	offset = anim_len * 4;
	for(u8 i6 = 0; i6 < anim_len; i6++) // black sprites
	{
		Mem_Copy(origData + (i6 * 2                + offset    ) * 8, dest, 8); dest += 8; // left up
		Mem_Copy(origData + (i6 * 2 + anim_len * 2 + offset    ) * 8, dest, 8); dest += 8; // left down
		Mem_Copy(origData + (i6 * 2                + offset + 1) * 8, dest, 8); dest += 8; // right up
		Mem_Copy(origData + (i6 * 2 + anim_len * 2 + offset + 1) * 8, dest, 8); dest += 8; // right down
	}
}

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
	VDP_SetPaletteEntry(0, RGB16(7, 7, 7)); // TRANSPARENT
	VDP_SetPaletteEntry(1, RGB16(0, 0, 0)); // BLACK
	VDP_SetPaletteEntry(2, RGB16(7, 6, 5)); // PLAYER
	VDP_SetPaletteEntry(3, RGB16(7, 3, 1)); // CARROT ORANGE
	VDP_SetPaletteEntry(4, RGB16(6, 6, 6)); // RABBIT WHITE
	VDP_SetPaletteEntry(5, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(6, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(7, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(8, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(9, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(10, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(11, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(12, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(13, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(14, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(15, RGB16(7, 7, 7));
	VDP_SetColor(15);
	VDP_SetPage(0);

	// Setup tilemap
	Tile_SetBankPage(2);
	Tile_FillBank(0, 6);
	Tile_FillBank(1, 7);
	Tile_FillBank(2, 8);
	Tile_FillBank(3, 9);
	Tile_LoadBank(0, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);
	Tile_LoadBank(2, g_DataBG4b, sizeof(g_DataBG4b) / TILE_CELL_BYTES);

	// Draw level
	Tile_SetDrawPage(0);
	Tile_SelectBank(0);
	Tile_FillScreen(6);
	Tile_DrawScreen(g_TileMap);

	// Setup and draw UI
	Print_SetBitmapFont(g_Font_MGL_Sample8);
	Print_SetColor(0x11, 0x66);
	Print_SetPosition(4, 4);
	Print_DrawText("TIME");
	Print_SetPosition(100, 4);
	Print_DrawText("SCORE");
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	VDP_HideSpriteFrom(8);

	// Load sprites in correct order to memory
	RearrangeSprites(g_carrot, g_CarrotSpriteData, 1);
	RearrangeSprites(g_rabbit, g_RabbitSpriteData, RABBIT_ANIMATION_FRAMES);
	RearrangeSprites(g_rabbit_dies, g_RabbitDiesSpriteData, RABBIT_ANIMATION_FRAMES);
	RearrangeSprites(g_rabbit_eats2, g_RabbitEatsSpriteData, RABBIT_ANIMATION_FRAMES);
	RearrangeSprites(g_player, g_PlayerSpriteData, PLAYER_ANIMATION_FRAMES);

	// Initialize 16x16 OR sprites
	
	// player
	VDP_SetSpriteExUniColor(0, 0, 0, 0 * 4, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 0, 1 * 4, VDP_SPRITE_CC + 0x01);

	// // rotating bersons
	// VDP_SetSpriteExUniColor(2, 0, 0, 2 * 4, 0x02);
	// VDP_SetSpriteExUniColor(3, 0, 0, 3 * 4, VDP_SPRITE_CC + 0x01);
	// VDP_SetSpriteExUniColor(4, 0, 0, 4 * 4, 0x02);
	// VDP_SetSpriteExUniColor(5, 0, 0, 5 * 4, VDP_SPRITE_CC + 0x01);
	// VDP_SetSpriteExUniColor(6, 0, 0, 6 * 4, 0x02);
	// VDP_SetSpriteExUniColor(7, 0, 0, 7 * 4, VDP_SPRITE_CC + 0x01);

	// bullets
	const int carrot_index = 8;
	const int y0 = 211;
	for (i = 0; i < MAX_CARROTS_IN_CARRY; ++i )
	{
		VDP_SetSpriteExUniColor(carrot_index + i,     0, y0, (carrot_index + i) * 4, 0x03);
		VDP_SetSpriteExUniColor(carrot_index + 1 + i, 0, y0, (carrot_index + i + 1) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_CarrotSpriteData,           (carrot_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_CarrotSpriteData + 4 * 8,   (carrot_index + i + 1) * 4, 4);
	}

	// rabbits
	int rabbit_index = carrot_index + MAX_CARROTS_IN_CARRY * 2;
	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		VDP_SetSpriteExUniColor(rabbit_index + i,     0, y0,               (rabbit_index + i) * 4, 0x04);
		VDP_SetSpriteExUniColor(rabbit_index + 1 + i, 0, y0,               (rabbit_index + 1 + i) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_RabbitSpriteData,                                   (rabbit_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_RabbitSpriteData + 4 * RABBIT_ANIMATION_FRAMES * 8, (rabbit_index + 1 + i) * 4, 4);
	}
	// rabbit dies
	rabbit_index = rabbit_index + RABBIT_COUNT * 2;
	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		VDP_SetSpriteExUniColor(rabbit_index + i,     0, y0,               (rabbit_index + i) * 4, 0x04);
		VDP_SetSpriteExUniColor(rabbit_index + 1 + i, 0, y0,               (rabbit_index + 1 + i) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_RabbitDiesSpriteData,                                   (rabbit_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_RabbitDiesSpriteData + 4 * RABBIT_ANIMATION_FRAMES * 8, (rabbit_index + 1 + i) * 4, 4);
	}
	// rabbit eats
	rabbit_index = rabbit_index + RABBIT_COUNT * 2;
	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		VDP_SetSpriteExUniColor(rabbit_index + i,     0, y0,               (rabbit_index + i) * 4, 0x04);
		VDP_SetSpriteExUniColor(rabbit_index + 1 + i, 0, y0,               (rabbit_index + 1 + i) * 4, VDP_SPRITE_CC + 0x01);
		VDP_LoadSpritePattern(g_RabbitEatsSpriteData,                                   (rabbit_index + i) * 4, 4);
		VDP_LoadSpritePattern(g_RabbitEatsSpriteData + 4 * RABBIT_ANIMATION_FRAMES * 8, (rabbit_index + 1 + i) * 4, 4);
	}


	// Compute transformed sprite data
	// loop(i, 6 * 2)
	// {
	// 	u16 idx = i * 4 * 8;
	// 	SpriteFX_RotateRight16(&g_PatternData[idx], &g_PatternDataRotRight[idx]);
	// 	SpriteFX_RotateLeft16(&g_PatternData[idx], &g_PatternDataRotLeft[idx]);
	// 	SpriteFX_RotateHalfTurn16(&g_PatternData[idx], &g_PatternDataRotHalf[idx]);
	// }

	loop(i, 1 * 2)
	{
		u16 idx = i * 4 * 8;
		SpriteFX_RotateRight16(&g_CarrotSpriteData[idx], &g_CarrotSpriteDataRotRight[idx]);
		SpriteFX_RotateLeft16(&g_CarrotSpriteData[idx], &g_CarrotSpriteDataRotLeft[idx]);
		SpriteFX_RotateHalfTurn16(&g_CarrotSpriteData[idx], &g_CarrotSpriteDataRotHalf[idx]);
	}

	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (0*256*8), 32*4*8);
	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (1*256*8), 32*4*8);
	// VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (2*256*8), 32*4*8);
}

//-----------------------------------------------------------------------------
void UpdateDraw()
{
	// DRAW PLAYER

	u8 frame = (g_Frame >> 2) % PLAYER_ANIMATION_FRAMES;
	u8 pat = (frame * 8 * 4);
	u8* pat1 = g_PlayerSpriteData + pat;
	u8* pat2 = g_PlayerSpriteData + pat + PLAYER_ANIMATION_FRAMES * 4 * 8;

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

	frame = (g_Frame >> 2) % RABBIT_ANIMATION_FRAMES;
	pat = (frame * 8 * 4);


	pat1 = g_RabbitSpriteData + pat;
	pat2 = g_RabbitSpriteData + pat + RABBIT_ANIMATION_FRAMES * 4 * 8;

	for (i = 0; i < RABBIT_COUNT; ++i )
	{
		if (rabbits[i].state == 3) // dies
		{
			pat1 = g_RabbitDiesSpriteData + pat;
			pat2 = g_RabbitDiesSpriteData + pat + RABBIT_ANIMATION_FRAMES * 4 * 8;
		}
		else if (rabbits[i].state == 1) // eats
		{
			pat1 = g_RabbitEatsSpriteData + pat;
			pat2 = g_RabbitEatsSpriteData + pat + RABBIT_ANIMATION_FRAMES * 4 * 8;
		}

		VDP_SetSpritePosition(14 + i, rabbits[i].pos.x, rabbits[i].pos.y);
		VDP_SetSpritePosition(15 + i, rabbits[i].pos.x, rabbits[i].pos.y);
		// VDP_LoadSpritePattern(pat1, 14, 4);
		// VDP_LoadSpritePattern(pat2, 14 + 1 * 4, 4);
		if(rabbits[i].flipHorizontal)
		{
			SpriteFX_FlipHorizontal16(pat1, g_Buffer3);
			SpriteFX_FlipHorizontal16(pat2, g_Buffer4);
			VDP_LoadSpritePattern(g_Buffer3, (14 + i) * 4, 4);
			VDP_LoadSpritePattern(g_Buffer4, (15 + i) * 4, 4);
		}
		else {
			VDP_LoadSpritePattern(pat1, (14 + i) * 4, 4);
			VDP_LoadSpritePattern(pat2, (15 + i) * 4, 4);
		}
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
			
			u8 rot = (g_Frame >> 2) % 4;
			VDP_LoadSpritePattern(g_CarRotAnim[rot], 		      (8 + i) * 4, 4);
			VDP_LoadSpritePattern(g_CarRotAnim[rot] + 4 * 8, 	  (9 + i) * 4, 4);
		}
		else
		{
			VDP_HideSprite(8 + i);
			VDP_HideSprite(9 + i);
		}
	}
}


void UpdateTimeUI()
{
	Print_SetPosition(40, 4);
	Print_DrawInt(timeLeft);
}

void UpdateScoreUI()
{
	Print_SetPosition(148, 4);
	Print_DrawInt(score);
}

void UpdateCarrotUI()
{
	Print_SetPosition(100, 14);
	if(player.carrots == 0)
	{
		Print_DrawText("CARROTS 0/3");
	}
	else if(player.carrots == 1)
	{
		Print_DrawText("CARROTS 1/3");
	}
	else if(player.carrots == 2)
	{
		Print_DrawText("CARROTS 2/3");
	}
	else if(player.carrots == 3)
	{
		Print_DrawText("CARROTS 3/3");
	}
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
		WaitVBlank();
		FSM_Update();
		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}